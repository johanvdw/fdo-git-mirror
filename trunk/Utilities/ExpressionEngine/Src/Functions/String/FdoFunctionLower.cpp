//
// 
// Copyright (C) 2004-2007  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 
//

#include <stdafx.h>
#include <Functions/String/FdoFunctionLower.h>
#include <FdoCommonOSUtil.h>

// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionLower::FdoFunctionLower ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;
    tmp_buffer = NULL;
    first = true;

}  //  FdoFunctionLower ()


FdoFunctionLower::~FdoFunctionLower ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

    delete [] tmp_buffer;

}  //  ~FdoFunctionLower ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionLower *FdoFunctionLower::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionLower();

}  //  Create ()

FdoFunctionLower *FdoFunctionLower::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionLower();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionLower::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function LOWER.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionLower::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function LOWER.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoStringValue> string_value;

    if (first)
    {
        Validate(literal_values);
        return_string_value    = FdoStringValue::Create();
        tmp_buffer      = new wchar_t[INIT_ALLOCATE_SIZE+1];
        tmp_buffer_size = INIT_ALLOCATE_SIZE;
        first = false;
    }

    // Process the request and return the result back to the calling routine.

    string_value = (FdoStringValue *) literal_values->GetItem(0);
    if (!string_value->IsNull()) {

        size_t size = wcslen(string_value->GetString());
        if (size > tmp_buffer_size) {
            delete [] tmp_buffer;
            tmp_buffer_size = size;
            tmp_buffer = new wchar_t[tmp_buffer_size + 1];

        }
        wcscpy(tmp_buffer, string_value->GetString());
        
        return_string_value->SetString(FdoCommonOSUtil::wcslwr(tmp_buffer));

    }  //  if (!string_value->IsNull()) ...
    else
       return_string_value->SetString(L"");
    return FDO_SAFE_ADDREF(return_string_value.p);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionLower::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function LOWER. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    LOWER (string)
// |
// | The function always returns a STRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              str_arg_literal;

    FdoPtr<FdoArgumentDefinition>           str_arg;

    FdoPtr<FdoArgumentDefinitionCollection> str_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                    FUNCTION_LOWER_STRING_ARG,
                                    "String to be converted into lowercase");

    // The following defines the different argument definition collections.

    str_arg_literal = FdoException::NLSGetMessage(FUNCTION_STRING_ARG_LIT,
                                                  "text property");

    str_arg = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);

    str_args = FdoArgumentDefinitionCollection::Create();
    str_args->Add(str_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_String, str_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
         FdoException::NLSGetMessage(
                FUNCTION_LOWER,
                "Converts all uppercase letters in a string expression into lowercase letters");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_LOWER,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_String);

}  //  CreateFunctionDefinition ()

void FdoFunctionLower::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                count           = literal_values->GetCount();

    FdoDataType             data_type;

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. LOWER accepts exactly one parameter. If
    // the number of parameters is not correct issue an exception.

    if (count != 1) 
        throw FdoException::Create(
               FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_LOWER));

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    literal_value = literal_values->GetItem(0);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_LOWER));

    data_value = static_cast<FdoDataValue *>(literal_value.p);
    data_type = data_value->GetDataType();
    if (data_type != FdoDataType_String)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_LOWER));

}  //  Validate ()

