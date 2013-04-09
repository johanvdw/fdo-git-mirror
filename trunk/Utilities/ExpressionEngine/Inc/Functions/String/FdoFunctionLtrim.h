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
#ifndef FDOFUNCTIONLTRIM_H
#define FDOFUNCTIONLTRIM_H

#include <FdoExpressionEngineINonAggregateFunction.h>


// ============================================================================
// The class FdoFunctionLtrim implements the Expression Engine function LTRIM.
// ============================================================================

class FdoFunctionLtrim : public FdoExpressionEngineINonAggregateFunction
{

    static const FdoInt32 INIT_ALLOCATE_SIZE = 100;

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionLtrim *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionLtrim *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function
        //  LTRIM. The definition includes the list of supported signatures
        //  for the function.

        virtual FdoFunctionDefinition *GetFunctionDefinition ();

        // Evaluate:
        //  The function determines the function result and returns it back to
        //  the calling routine.

        virtual FdoLiteralValue *Evaluate (
                                    FdoLiteralValueCollection *literal_values);


    private:

        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionLtrim:
        //  The function represents the class constructor.

        FdoFunctionLtrim ();

        // ~FdoFunctionLtrim:
        //  The function represents the class destructor.

        ~FdoFunctionLtrim ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function LTRIM.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // Validate:
        //  The function validates the provided parameters for the function
        //  LTRIM.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  LTRIM.

        FdoFunctionDefinition *function_definition;

        FdoPtr<FdoStringValue> return_string_value;

        wchar_t *tmp_buffer;

        size_t tmp_buffer_size;

        bool first;

};  //  class FdoFunctionLtrim

#endif
