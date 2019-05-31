//
//  native_plus.cpp
//  native_plus
//
//  Created by 潘万坤 on 2019/5/31.
//  Copyright © 2019 潘万坤. All rights reserved.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "include/dart_api.h"
#include "include/dart_native_api.h"


Dart_NativeFunction ResolveName(Dart_Handle name,
                                int argc,
                                bool* auto_setup_scope);


DART_EXPORT Dart_Handle native_plus_Init(Dart_Handle parent_library) {
    if (Dart_IsError(parent_library)) {
        return parent_library;
    }
    
    Dart_Handle result_code =
    Dart_SetNativeResolver(parent_library, ResolveName, NULL);
    if (Dart_IsError(result_code)) {
        return result_code;
    }
    
    return Dart_Null();
}

Dart_Handle HandleError(Dart_Handle handle) {
    if (Dart_IsError(handle)) {
        Dart_PropagateError(handle);
    }
    return handle;
}

void NativePlus(Dart_NativeArguments args) {
    Dart_EnterScope();
    int64_t result = 0;
    Dart_Handle plus_a = HandleError(Dart_GetNativeArgument(args, 0));
    Dart_Handle plus_b = HandleError(Dart_GetNativeArgument(args, 1));
    if (Dart_IsInteger(plus_a) && Dart_IsInteger(plus_b)) {
        int64_t a,b;
        HandleError(Dart_IntegerToInt64(plus_a, &a));
        HandleError(Dart_IntegerToInt64(plus_b, &b));
        result = a + b;
    }
    Dart_SetReturnValue(args, HandleError(Dart_NewInteger(result)));
    Dart_ExitScope();
}

struct FunctionLookup {
    const char* name;
    Dart_NativeFunction function;
};

FunctionLookup function_list[] = {
    {"NativePlus", NativePlus},
    {NULL, NULL}};

Dart_NativeFunction ResolveName(Dart_Handle name,
                                int argc,
                                bool* auto_setup_scope) {
    if (!Dart_IsString(name)) {
        return NULL;
    }
    Dart_NativeFunction result = NULL;
    if (auto_setup_scope == NULL) {
        return NULL;
    }
    
    Dart_EnterScope();
    const char* cname;
    HandleError(Dart_StringToCString(name, &cname));
    
    for (int i=0; function_list[i].name != NULL; ++i) {
        if (strcmp(function_list[i].name, cname) == 0) {
            *auto_setup_scope = true;
            result = function_list[i].function;
            break;
        }
    }
    
    if (result != NULL) {
        Dart_ExitScope();
        return result;
    }
    
    Dart_ExitScope();
    return result;
}
