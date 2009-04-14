/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(XALANXPATHEXCEPTION_HEADER_GUARD_1357924680)
#define XALANXPATHEXCEPTION_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;



class XALAN_XPATH_EXPORT XalanXPathException : public XSLException
{
public:

	/**
	 * Construct an XPath exeption object.
	 * 
	 * @param message message explaining the problem. 
	 * @param theURI the URI of the related document, if known
	 * @param theLineNumber the line number of the related document.
	 * @param theColumnNumber the column number of the related document.
	 * @param styleNode the node in the stylesheet where the problem occurred
	 * @param theType type of exception, default is "XalanXPathException"
	 */
	XalanXPathException(
			const XalanDOMString&	message,
			const XalanDOMString&	theURI,
			int						theLineNumber,
			int						theColumnNumber,
			const XalanNode*		styleNode = 0,
			const XalanDOMString&	theType = XalanDOMString(XALAN_STATIC_UCODE_STRING("XalanXPathException")));

	/**
	 * Constructor
	 * 
	 * @param theLocator The locator instance for error reporting.
	 * @param theMessage message to write when exception thrown
	 * @param styleNode the node in the stylesheet where the problem occurred
	 * @param theType type of exception, default is "XalanXPathException"
	 */
	XalanXPathException(
			const LocatorType&		theLocator,
			const XalanDOMString&	theMessage,
			const XalanNode*		styleNode = 0,
			const XalanDOMString&	theType = XalanDOMString(XALAN_STATIC_UCODE_STRING("XalanXPathException")));

	/**
	 * Construct an XPath exeption object.
	 * 
	 * @param message message explaining the problem. 
	 * @param styleNode the node in the stylesheet where the problem occurred
	 * @param theType type of exception, default is "XalanXPathException"
	 */
	XalanXPathException(
			const XalanDOMString&	message,
			const XalanNode*		styleNode = 0,
			const XalanDOMString&	theType = XalanDOMString(XALAN_STATIC_UCODE_STRING("XalanXPathException")));

	virtual
	~XalanXPathException();

	const XalanNode*
	getStyleNode() const
	{
		return m_styleNode;
	}

protected:

	/**
	 * Construct an XPath exeption object.
	 * 
	 */
	explicit
	XalanXPathException();

private:

	const XalanNode*	m_styleNode;

	static const XalanDOMString		s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANXPATHEXCEPTION_HEADER_GUARD_1357924680
