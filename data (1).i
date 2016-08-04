 
 
 
 
 
 
 
 once
 _INC_STRING

 
 
 
 
 
 
 
 once



_CRT_BEGIN_C_HEADER



 
 
 
 
 
 _ACRTIMP
     defined _CRTIMP & !defined _VCRT_DEFINED_CRTIMP
         _ACRTIMP _CRTIMP
     !defined _CORECRT_BUILD & defined _DLL
         _ACRTIMP __declspec(dllimport)
    
         _ACRTIMP
    


 
 
 _ACRTIMP_ALT
     _ACRTIMP_ALT _ACRTIMP


 _DCRTIMP
     defined _CRTIMP & !defined _VCRT_DEFINED_CRTIMP
         _DCRTIMP _CRTIMP
     !defined _CORECRT_BUILD & defined _DLL
         _DCRTIMP __declspec(dllimport)
    
         _DCRTIMP
    


 defined _CRT_SUPPRESS_RESTRICT | defined _CORECRT_BUILD
     _CRTRESTRICT

     _CRTRESTRICT __declspec(restrict)


 _MSC_VER > 1900 & !defined __EDG__ & !defined _CORECRT_BUILD
     _CRTALLOCATOR __declspec(allocator)

     _CRTALLOCATOR


 defined _M_CEE & defined _M_X64
     
     
     _CRT_JIT_INTRINSIC __declspec(jitintrinsic)

     _CRT_JIT_INTRINSIC


 
 _GUARDOVERFLOW_CRT_ALLOCATORS
     _CRT_GUARDOVERFLOW __declspec(guard(overflow))

     _CRT_GUARDOVERFLOW 


 _CONST_RETURN
     __cplusplus
         _CONST_RETURN const
         _CRT_CONST_CORRECT_OVERLOADS
    
         _CONST_RETURN
    


 _WConst_return _CONST_RETURN  

 _CRT_ALIGN
     __midl
         _CRT_ALIGN(x)
    
         _CRT_ALIGN(x) __declspec(align(x))
    


 defined _PREFAST_ & defined _CA_SHOULD_CHECK_RETURN
     _Check_return_opt_ _Check_return_

     _Check_return_opt_


 defined _PREFAST_ & defined _CA_SHOULD_CHECK_RETURN_WER
     _Check_return_wat_ _Check_return_

     _Check_return_wat_


 !defined __midl & !defined MIDL_PASS & defined _PREFAST_
