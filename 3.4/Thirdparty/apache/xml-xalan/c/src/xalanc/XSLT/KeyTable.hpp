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
#if !defined(XALAN_KEYTABLE_HEADER_GUARD)
#define XALAN_KEYTABLE_HEADER_GUARD 

/**
 * $Id: KeyTable.hpp,v 1.3 2004/01/06 02:41:36 dbertoni Exp $
 * 
 * $State: Exp $
 * 
 */



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#if defined(XALAN_USE_HASH_MAP)
#include <hash_map>
#else
#include <map>
#endif
#include <vector>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/XPath/MutableNodeRefList.hpp>
#include <xalanc/XPath/XalanQNameByReference.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class KeyDeclaration;
class NodeRefListBase;
class PrefixResolver;
class StylesheetExecutionContext;
class XalanElement;
class XalanDocument;
class XalanNode;




/**
 * Table of element keys, keyed by document node.  An instance of this 
 * class is keyed by a Document node that should be matched with the 
 * root of the current context.  It contains a table of name mappings 
 * to tables that contain mappings of identifier values to nodes.
 */
class KeyTable
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<KeyDeclaration>			KeyDeclarationVectorType;

	typedef map<XalanDOMString,
				MutableNodeRefList,
				less<XalanDOMString> >		NodeListMapType;

	typedef map<XalanQNameByReference,
				NodeListMapType,
				less<XalanQNameByReference> >	KeysMapType;
#else
	typedef std::vector<KeyDeclaration>		KeyDeclarationVectorType;

#if defined(XALAN_USE_HASH_MAP)
	typedef std::hash_map<XalanDOMString,
						  MutableNodeRefList>	NodeListMapType;

	typedef std::hash_map<XalanQNameByReference,
						  NodeListMapType>		KeysMapType;
#else
	typedef std::map<XalanDOMString,
					 MutableNodeRefList>	NodeListMapType;

	typedef std::map<XalanQNameByReference,
					 NodeListMapType>		KeysMapType;
#endif
#endif


	/**
	 * Build a keys table.
	 *
	 * @param startNode        node to start iterating from to build the keys
	 *                         index
	 * @param nscontext        stylesheet's namespace context
	 * @param keyDeclarations  stylesheet's xsl:key declarations
	 * @param executionContext current execution context
	 */
	KeyTable(
			XalanNode*							startNode,
			const PrefixResolver&				resolver,
			const KeyDeclarationVectorType&		keyDeclarations,
			StylesheetExecutionContext&			executionContext);

	virtual
	~KeyTable();

	/**
	 * Given a valid element key, return the corresponding node list. If the
	 * name was not declared with xsl:key, this will return null, the
	 * identifier is not found, it will return an empty node set, otherwise it
	 * will return a nodeset of nodes.
	 *
	 * @param name name of the key, which must match the 'name' attribute on
	 *             xsl:key
	 * @param ref  value that must match the value found by the 'match'
	 *             attribute on xsl:key
	 * @return		pointer to nodeset for key 
	 */
	const MutableNodeRefList&
	getNodeSetByKey(
				  const XalanQName&			qname,
				  const XalanDOMString&		ref) const;

private:

	static void
	processKeyDeclaration(
			KeysMapType&					theKeys,
			const KeyDeclaration&			kd,
			XalanNode*						testNode,
			const PrefixResolver&			resolver,
			StylesheetExecutionContext&		executionContext);

	/**
	 * The document key.  This table should only be used with contexts
	 * whose Document roots match this key.
	 */
	const XalanDocument*	m_docKey;

	/**
	 * Table of element keys.  The table will be built on demand, 
	 * when a key is requested, or set by the XMLParserLiaison or 
	 * the caller.  The table is:
	 * a) keyed by name,
	 * b) each with a value of a hashtable, keyed by the value returned by 
	 *    the use attribute,
	 * c) each with a value that is a nodelist.
	 * Thus, for a given key or keyref, look up hashtable by name, 
	 * look up the nodelist by the given reference.
	 */

	KeysMapType							m_keys;

	static const MutableNodeRefList		s_dummyList;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_KEYTABLE_HEADER_GUARD
