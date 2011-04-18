/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <stdafx.h>
#include <FdoCommonStringUtil.h>

#pragma warning(push)
#pragma warning(disable: 4275)  // Disable warning C4275: non dll-interface class
#pragma warning(disable: 4251)  // Disable warning C4251: needs to have dll-interface to be used by clients of class
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#pragma warning(pop) // Enable warnings

#include "FdoOwsHttpHandler.h"
#include <FdoCommonOSUtil.h>
#include <OWS/FdoOwsGlobals.h>

FdoOwsHttpHandler::FdoOwsHttpHandler()   :
                            m_url(NULL), m_bGet(false), m_parameters(NULL),
                            m_proxyHost(NULL), m_proxyPort(NULL), m_proxyUserName(NULL), m_proxyPassword(NULL),
                            m_connectionState(ConnectionState_BeforeConnect),
                            m_disposed(false), m_userName(NULL), m_passwd(NULL),
                            m_bValidDocument(false), m_contentType(FdoOwsMIMEType_unknown),
                            m_currentSize(0), m_currentRead(0), m_bRunning(false)
{
    m_errorBuffer[0] = '\0';
    m_tvConnect = 0;
}

FdoOwsHttpHandler::FdoOwsHttpHandler(const char* url, bool bGet, const char* parameters, 
                         const char* userName, const char* passwd, const char* proxyHost, const char* proxyPort, const char* proxyUserName, const char* proxyPassword) :
                            m_url(url), m_bGet(bGet), m_parameters(parameters), m_connectionState(ConnectionState_BeforeConnect),
                            m_disposed(false), m_userName(userName), m_passwd(passwd),
                            m_proxyHost(proxyHost), m_proxyPort(proxyPort), m_proxyUserName(proxyUserName), m_proxyPassword(proxyPassword), 
                            m_bValidDocument(false), m_contentType(FdoOwsMIMEType_unknown),
                            m_currentSize(0), m_currentRead(0), m_bRunning(false)
{
    m_errorBuffer[0] = '\0';
    m_tvConnect = 0;    
}

FdoOwsHttpHandler::FdoOwsHttpHandler(const char* url, bool bGet, const char* parameters, 
                         const char* userName, const char* passwd) :
                            m_url(url), m_bGet(bGet), m_parameters(parameters),
                            m_connectionState(ConnectionState_BeforeConnect),
                            m_disposed(false), m_userName(userName), m_passwd(passwd),
                            m_bValidDocument(false), m_contentType(FdoOwsMIMEType_unknown),
                            m_currentSize(0), m_currentRead(0), m_bRunning(false)
{
    m_errorBuffer[0] = '\0';
    m_tvConnect = 0;    
}

FdoOwsHttpHandler::~FdoOwsHttpHandler()
{
    size_t size = m_contents.size();
    for (size_t i = 0; i < size; i++)
        delete[] m_contents[i];
}

void FdoOwsHttpHandler::Dispose()
{
    {
        boost::mutex::scoped_lock lock(m_mutex);
        m_disposed = true;
        // wait till the thread stops
        while (m_bRunning)
            m_condition.wait(lock);
    }

	if (m_thread.get())
		m_thread->join ();
    delete this;
}

FdoOwsHttpHandler* FdoOwsHttpHandler::Create(const char* url, bool bGet, const char* parameters,
    const char* userName, const char* passwd, const char* proxyHost, const char* proxyPort, const char* proxyUserName, const char* proxyPassword)
{
    return new FdoOwsHttpHandler(url, bGet, parameters, userName, passwd, proxyHost, proxyPort, proxyUserName, proxyPassword);
}

size_t FdoOwsHttpHandler::HeaderCallback( void *ptr, size_t size, size_t nmemb, void *stream)
{
    return reinterpret_cast<FdoOwsHttpHandler*>(stream)->_headerCallback(ptr, size, nmemb);
}

size_t FdoOwsHttpHandler::WriteCallback( void *ptr, size_t size, size_t nmemb, void *stream)
{
    return reinterpret_cast<FdoOwsHttpHandler*>(stream)->_writeCallback(ptr, size, nmemb);
}

size_t FdoOwsHttpHandler::_headerCallback( void *ptr, size_t size, size_t nmemb)
{
    assert(!m_disposed);
    size_t len = size * nmemb;
    if (!m_bValidDocument)
    {
        // parse the HTTP return code and find out 
        // whether current document is valid
        if (len > FdoOwsGlobals::MIN_LEN_HTTP_RESPONSE_FIRST_LINE && 
            strncmp((const char*)ptr, FdoOwsGlobals::HTTP_SLASH, FdoOwsGlobals::HTTP_SLASH_LEN) == 0)
        {
            char retCodeStr[FdoOwsGlobals::LEN_HTTP_RET_CODE+1];
            strncpy(retCodeStr, 
                (const char*)ptr + FdoOwsGlobals::MIN_LEN_HTTP_RESPONSE_FIRST_LINE - FdoOwsGlobals::LEN_HTTP_RET_CODE, 
                FdoOwsGlobals::LEN_HTTP_RET_CODE);
            retCodeStr[FdoOwsGlobals::LEN_HTTP_RET_CODE] = '\0';
            int retCode = atoi(retCodeStr);
            if (retCode < FdoOwsGlobals::MIN_HTTP_ERROR_CODE)
                m_bValidDocument = true;
        }
    }
    else
    {
        // try to find out the content type
        size_t nContentType = FdoOwsGlobals::Content_Type_Len;
        if (len > nContentType && 
            FdoCommonOSUtil::strnicmp((const char*)ptr, FdoOwsGlobals::Content_Type, nContentType) == 0)
        {
            // trim the blank(s) and ':';
            size_t pt = nContentType;
            while (pt < len &&
                (((const char*)ptr)[pt] == FdoOwsGlobals::Space ||
                ((const char*)ptr)[pt] == FdoOwsGlobals::Colon))
                pt++;
            if (pt < len)
            {
                const char* typeString = (const char*)ptr + pt;
                size_t nType = len - pt;
                if (nType >= FdoOwsGlobals::text_xml_len &&
                    FdoCommonOSUtil::strnicmp(typeString, FdoOwsGlobals::text_xml, FdoOwsGlobals::text_xml_len) == 0)
                    m_contentType = FdoOwsMIMEType_text_xml;
				else if (nType >= FdoOwsGlobals::image_png_len &&
					FdoCommonOSUtil::strnicmp(typeString, FdoOwsGlobals::image_png, FdoOwsGlobals::image_png_len) == 0)
					m_contentType = FdoOwsMIMEType_image_png;
				else if (nType >= FdoOwsGlobals::image_jpeg_len &&
					FdoCommonOSUtil::strnicmp(typeString, FdoOwsGlobals::image_jpeg, FdoOwsGlobals::image_jpeg_len) == 0)
					m_contentType = FdoOwsMIMEType_image_jpeg;
				else if (nType >= FdoOwsGlobals::image_tiff_len &&
					FdoCommonOSUtil::strnicmp(typeString, FdoOwsGlobals::image_tiff, FdoOwsGlobals::image_tiff_len) == 0)
					m_contentType = FdoOwsMIMEType_image_tiff;
            }

        }


    }
    return len;

}

size_t FdoOwsHttpHandler::_writeCallback( void *ptr, size_t size, size_t nmemb)
{
    // this function may be called by multi threads
    boost::mutex::scoped_lock lock(m_mutex);
    if (m_disposed) // this object is already disposed by the main thread
        return 0;
    size_t len = size * nmemb;
    if (!m_bValidDocument)
        return len;
    if (len > 0)
    {
        if (m_contents.size() == 0)
        {
            // we are first here, change the state
            m_connectionState = ConnectionState_Connected;
            m_condition.notify_all();
        }
        char* content = new char[len];
        memcpy(content, ptr, len);
        m_contents.push_back(content);
        m_contentSizes.push_back(len);
        m_currentSize += len;
        m_condition.notify_all(); // someone else may be waiting for the content
    }


    return len;
}

FdoException* FdoOwsHttpHandler::_translateError(CURLcode curlCode, FdoString *error_msg)
{
    FdoException* e;

    switch (curlCode)
    {
    case CURLE_UNSUPPORTED_PROTOCOL:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_164_UNSUPPORTED_HTTP_PROTOCOL)));
        break;
    case CURLE_URL_MALFORMAT:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_165_MALFORMED_URL)));
        break;
    case CURLE_COULDNT_RESOLVE_PROXY:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_166_UNABLE_RESOLVE_PROXY)));
        break;
    case CURLE_COULDNT_RESOLVE_HOST:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_163_UNABLE_RESOLVE_HOST)));
        break;
    case CURLE_COULDNT_CONNECT:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_161_UNABLE_CONNECT_HOST)));
        break;
    case CURLE_PARTIAL_FILE:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_167_DOWNLOAD_PARTIAL_FILE)));
        break;
    case CURLE_READ_ERROR:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_168_FAILURE_OPEN_FILE)));
        break;
    case CURLE_OUT_OF_MEMORY:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(CLNT_5_OUTOFMEMORY)));
        break;
    case CURLE_OPERATION_TIMEOUTED:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_162_CONNECT_HOST_TIMEOUT)));
        break;
    case CURLE_HTTP_POST_ERROR:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_169_FAILURE_POST)));
        break;
    case CURLE_SSL_CONNECT_ERROR:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_170_FAILURE_SSL_CONNECT)));
        break;
    case CURLE_BAD_DOWNLOAD_RESUME:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_171_UNABLE_RESUME_DOWNLOAD)));
        break;
    case CURLE_FILE_COULDNT_READ_FILE:  
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_168_FAILURE_OPEN_FILE)));
        break;
    case CURLE_ABORTED_BY_CALLBACK:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_172_ABORTED_BY_CALLBACK)));
        break;
    case CURLE_TOO_MANY_REDIRECTS:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_173_ENDLESS_URL_REDIRECT)));
        break;
    case CURLE_SSL_PEER_CERTIFICATE:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_174_SSL_PEER_CERTIFICATE)));
        break;
    case CURLE_GOT_NOTHING:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_175_SERVER_RETURNED_NOTHING)));
        break;
    case CURLE_SEND_ERROR:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_176_FAILURE_SENDING_NETWORK_DATA)));
        break;
    case CURLE_RECV_ERROR:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_177_FAILURE_RECIEVING_NETWORK_DATA)));
        break;
    case CURLE_SSL_CERTPROBLEM:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_178_SSL_CERTIFICATE_ERROR)));
        break;
    case CURLE_SSL_CIPHER:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_179_SSL_CIPHER_ERROR)));
        break;
    case CURLE_SSL_CACERT:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_180_CACERT_CIPHER_ERROR)));
        break;
    case CURLE_LDAP_INVALID_URL:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_181_LDAP_INVALID_URL)));
        break;
    case CURLE_HTTP_RETURNED_ERROR:
        if (FdoCommonStringUtil::StringCompareNoCaseN(error_msg, L"The requested URL returned error:", 33) == 0) {
            FdoStringP str(error_msg);
            FdoStringP strErrorNum = str.Right(L": ");
            
            if (strErrorNum == L"400") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_145_URL_BAD_REQUEST)));
            }
            else if (strErrorNum == L"401") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_146_URL_NOT_AUTHORIZED)));
            }
            else if (strErrorNum == L"403") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_147_URL_ACCESS_DENIED)));
            }
            else if (strErrorNum == L"404") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_148_URI_NOT_FOUND)));
            }
            else if (strErrorNum == L"405") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_149_URL_REQUEST_NOT_ALLOWED)));
            }
            else if (strErrorNum == L"406") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_150_URL_REQUEST_NOT_ACCEPTABLE)));
            }
            else if (strErrorNum == L"407") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_151_URL_PROXY_AUTHENTICATION_REQUIRED)));
            }
            else if (strErrorNum == L"408") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_152_URL_REQUEST_TIMEOUT)));
            }
            else if (strErrorNum == L"410") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_153_URL_NOT_AVAILABLE)));
            }
            else if (strErrorNum == L"414") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_154_URL_REQUEST_TOO_LONG)));
            }
            else if (strErrorNum == L"500") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_155_URL_INTERNAL_SERVER_ERROR)));
            }
            else if (strErrorNum == L"501") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_156_URL_REQUEST_NOT_IMPLEMENTED)));
            }
            else if (strErrorNum == L"502") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_157_URL_OVERLOADED)));
            }
            else if (strErrorNum == L"503") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_158_URL_SERVICE_UNAVAILABLE)));
            }
            else if (strErrorNum == L"504") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_159_URL_GATEWAY_TIMEOUT)));
            }
            else if (strErrorNum == L"505") {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_160_HTTP_UNSUPPORTED_VERION)));
            }
            else
            {
                e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_133_UNEXPECTEDERROR_PERFORMING)));
                e->SetCause (FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_143_URL_ERROR), (FdoString*)strErrorNum)));
            }
        }
        else 
        {
            e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_133_UNEXPECTEDERROR_PERFORMING)));
            e->SetCause (FdoException::Create(error_msg));
        }

        break;
    default:
        e = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_133_UNEXPECTEDERROR_PERFORMING)));
        e->SetCause (FdoException::Create(error_msg));
    }

    return e;
}

void FdoOwsHttpHandler::Perform()
{
    // run this object
    m_thread = std::auto_ptr<boost::thread>(
        new boost::thread(boost::bind(&FdoOwsHttpHandler::Proc, this)));
    boost::mutex::scoped_lock lock(m_mutex);
    // wait for Receiving_Content
    m_condition.wait(lock);
    // check whether current state is expected
    if (m_connectionState == ConnectionState_BeforeConnect)
    {
        wchar_t* wError;
        multibyte_to_wide(wError, m_errorBuffer);
        throw _translateError(m_curlCode, wError);
    }
}


void FdoOwsHttpHandler::Proc()
{
    {
        boost::mutex::scoped_lock lock(m_mutex);
        m_bRunning = true;
        m_connectionState = ConnectionState_BeforeConnect;
    }
    CURL* curlHandle = curl_easy_init();

    // Explanation of error handling here:
    // It seems that we just totally ignored all libcurl error, but actually not.
    // Please note that FdoOwsHttpHandler::Proc is running in another thread, 
    // so how could we throw an exception from a working thread to the main thread? 
    // Actually whenever there is a libcurl error code returned, libcurl would put 
    // the error message in a buffer and what we did is just set the state of the 
    // http connection to proper value so that the main thread could detect that 
    // something is wrong. After that, it would generate the exception from the 
    // error buffer and throw it out  in the main thread so that the user can catch it.

    m_curlCode = CURLE_OK;
    CURLcode rv = CURL_LAST;
    for(;;) {
        if (curlHandle == NULL) break;
        
        // set error options
        rv = curl_easy_setopt(curlHandle, CURLOPT_ERRORBUFFER, m_errorBuffer);
        if (rv != CURLE_OK) break;
        rv = curl_easy_setopt(curlHandle, CURLOPT_FAILONERROR, CURL_TRUE);
        if (rv != CURLE_OK) break;

        // set behavior options
#ifdef _DEBUG // for debug
        rv = curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, CURL_TRUE);
        if (rv != CURLE_OK) break;
#endif
        rv = curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, CURL_TRUE);
        if (rv != CURLE_OK) break;

        // set network options
        std::string url = m_url;
        if (m_bGet && m_parameters.size() != 0)
        {
            // search "?" in the url, if not found append it
			if (url.find('?') == std::string::npos)
				url += '?';
			else
			{
                if (url[url.size() - 1] != '&' && url[url.size() - 1] != '?')
					url += '&';
			}
            url += m_parameters;
        }
		
        rv = curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
#ifdef _WIN32
		// this is extremely useful for debug in Mako, you could see in the VS output window 
		// the actual HTTP requests that are being sent out in the backend
		_RPT1(0, "\n%s\n", url.c_str()); 
#endif
        if (rv != CURLE_OK) break;

        // set authentication options
        std::string userPwd;
        if (m_userName.size() != 0)
        {
            userPwd = m_userName;
            userPwd += ':';
            userPwd += m_passwd;
            rv = curl_easy_setopt(curlHandle, CURLOPT_USERPWD, userPwd.c_str());
            if (rv != CURLE_OK) break;
            rv = curl_easy_setopt(curlHandle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
            if (rv != CURLE_OK) break;
        }
        
        // Set the proxy server options.
        // If the host name is empty, ignore the other proxy setting.
        if (!m_proxyHost.empty())
        {
            // By default, libcurl will default to using port 1080 for proxy.
            if (m_proxyPort.empty())
                m_proxyPort = "1080";
            std::string proxy = m_proxyHost + ':' + m_proxyPort;
            rv = curl_easy_setopt(curlHandle, CURLOPT_PROXY, proxy.c_str()); 
            if (rv != CURLE_OK)
                break;

            // Set the proxy user.
            if (!m_proxyUserName.empty())
            {
                rv = curl_easy_setopt(curlHandle, CURLOPT_PROXYUSERNAME, m_proxyUserName.c_str());
                if (rv != CURLE_OK) 
                    break;
            }
            // Set the proxy password.
            if (!m_proxyPassword.empty())
            {
                rv = curl_easy_setopt(curlHandle, CURLOPT_PROXYPASSWORD, m_proxyPassword.c_str());
                if (rv != CURLE_OK) 
                    break;
            }
        }

        // set http options
        rv = curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, CURL_TRUE);
        if (rv != CURLE_OK) break;
        rv = curl_easy_setopt(curlHandle, CURLOPT_UNRESTRICTED_AUTH, CURL_TRUE);
        if (rv != CURLE_OK) break;
        rv = curl_easy_setopt(curlHandle, CURLOPT_MAXREDIRS, 4);
        if (rv != CURLE_OK) break;
        curl_slist* headers = NULL;
        if (m_bGet)
        {
            rv = curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, CURL_TRUE);
            if (rv != CURLE_OK) break;
        }
        else
        {
            rv = curl_easy_setopt(curlHandle, CURLOPT_POST, CURL_TRUE);
            if (rv != CURLE_OK) break;
            // change the content type to text/xml
            headers = curl_slist_append(headers, "Content-Type: text/xml");
            rv = curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headers);
            if (rv != CURLE_OK) break;
            rv = curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, m_parameters.c_str());
            if (rv != CURLE_OK) break;
            rv = curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDSIZE, m_parameters.size());
            if (rv != CURLE_OK) break;
        }

        // Sets the timeout value. libcurl supports two kinds of timeout.
        // One is for "connection" timeout, the other is for "transfer" timeout.        
        // Option "CURLOPT_CONNECTIONTIMEOUT" is limitted to connection phase. 
        // In this phase name lookups may take serveral minutes.        
        if (m_tvConnect > 0)
        {
            rv = curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, m_tvConnect);
            if (rv != CURLE_OK) break;
        }
        else 
        {
            // disable the timeout
            rv = curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, 0);
            if (rv != CURLE_OK) break;
        }

        // set callback options
        rv = curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, &WriteCallback);
        if (rv != CURLE_OK) break;
        // use this as the pass-in parameter to write callback
        rv = curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, this);
        if (rv != CURLE_OK) break;
        rv = curl_easy_setopt(curlHandle, CURLOPT_HEADERFUNCTION, &HeaderCallback);
        if (rv != CURLE_OK) break;
        rv = curl_easy_setopt(curlHandle, CURLOPT_WRITEHEADER, this);

#ifndef FALSE
#define FALSE 0
#endif
		// don't verify the peer (server)
		curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, FALSE);

        // oops, a lot of options set
        // now we can perform
        rv = curl_easy_perform(curlHandle);
        if (rv != CURLE_OK) break;

        // we are done, set the state to Terminated
        boost::mutex::scoped_lock lock(m_mutex);
        m_connectionState = ConnectionState_Terminated;
        break;
    } // end of for (;;)

    if (rv != CURLE_OK) {
        // error occurs, set the state to BeforeConnect
        m_curlCode = rv;
        boost::mutex::scoped_lock lock(m_mutex);
        m_connectionState = ConnectionState_BeforeConnect;
    }

    if (curlHandle != NULL)
        curl_easy_cleanup(curlHandle);

    
    {
        boost::mutex::scoped_lock lock(m_mutex);
        // we still need to nofity other processes in case it is waiting for us
        m_condition.notify_all();
        m_bRunning = false;
    }

}


FdoSize FdoOwsHttpHandler::Read( FdoByte* buffer, FdoSize toRead )
{
    boost::mutex::scoped_lock lock(m_mutex);
    while (m_currentRead >= m_currentSize) // no data available, we must wait if still connected
    {
		if (m_connectionState == ConnectionState_BeforeConnect) // something bad happened
			throw FdoException::Create(FdoException::NLSGetMessage(FDO_132_CONNECTION_BROKEN, "Network connection is broken, or the server stopped responding."));
        if (m_connectionState == ConnectionState_Terminated)
			return 0;
        m_condition.wait(lock);
    }
    size_t block = 0;
    size_t offset = m_currentRead;
    while (offset >= m_contentSizes[block])
        offset -= m_contentSizes[block++];
    size_t numRead = 0;
    while (toRead > 0 && block < m_contents.size())
    {
        size_t numReadBlock = m_contentSizes[block] - offset > toRead ? toRead :
                                m_contentSizes[block] - offset;
        memcpy(buffer + numRead, m_contents[block] + offset, numReadBlock);
        numRead += numReadBlock;
        toRead -= numReadBlock;
        offset += numReadBlock;
        if (offset == m_contentSizes[block])
        {
            offset = 0;
            block++;
        }
    }
    m_currentRead += numRead;
    return numRead;

}

void FdoOwsHttpHandler::Write( FdoByte* buffer, FdoSize count )
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED), "Method is not implemented."));
}

void FdoOwsHttpHandler::Write( FdoIoStream* stream, FdoSize count)
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED), "Method is not implemented."));
}

void FdoOwsHttpHandler::SetLength( FdoInt64 length )
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED), "Method is not implemented."));
}

FdoInt64 FdoOwsHttpHandler::GetLength()
{
    return (FdoInt64)-1;
}

FdoInt64 FdoOwsHttpHandler::GetIndex()
{
    boost::mutex::scoped_lock lock(m_mutex);
    return m_currentRead;
}

void FdoOwsHttpHandler::Skip( FdoInt64 offset )
{
    boost::mutex::scoped_lock lock(m_mutex);
    if (offset < 0 && -offset > m_currentRead)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INDEXOUTOFBOUNDS)));
    m_currentRead += (size_t)offset;
}

void FdoOwsHttpHandler::Reset()
{
    boost::mutex::scoped_lock lock(m_mutex);
    m_currentRead = 0;
}

FdoBoolean FdoOwsHttpHandler::CanRead()
{
    return true;
}

FdoBoolean FdoOwsHttpHandler::CanWrite()
{
    return false;
}

FdoBoolean FdoOwsHttpHandler::HasContext()
{
    return true;
}

void FdoOwsHttpHandler::SetConnectionTimeout (unsigned int seconds)
{
    m_tvConnect = seconds;
}

unsigned int FdoOwsHttpHandler::GetConnectionTimeout (void)
{
    return m_tvConnect;
}
