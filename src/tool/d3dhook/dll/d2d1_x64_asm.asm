.code
extern mProcs:QWORD
D2D1ComputeMaximumScaleFactor_wrapper proc
	jmp mProcs[0*8]
D2D1ComputeMaximumScaleFactor_wrapper endp
D2D1ConvertColorSpace_wrapper proc
	jmp mProcs[1*8]
D2D1ConvertColorSpace_wrapper endp
;;D2D1CreateDevice_wrapper proc
;;	jmp mProcs[2*8]
;;D2D1CreateDevice_wrapper endp
;;D2D1CreateDeviceContext_wrapper proc
;;	jmp mProcs[3*8]
;;D2D1CreateDeviceContext_wrapper endp
;;D2D1CreateFactory_wrapper proc
;;	jmp mProcs[4*8]
;;D2D1CreateFactory_wrapper endp
D2D1InvertMatrix_wrapper proc
	jmp mProcs[5*8]
D2D1InvertMatrix_wrapper endp
D2D1IsMatrixInvertible_wrapper proc
	jmp mProcs[6*8]
D2D1IsMatrixInvertible_wrapper endp
D2D1MakeRotateMatrix_wrapper proc
	jmp mProcs[7*8]
D2D1MakeRotateMatrix_wrapper endp
D2D1MakeSkewMatrix_wrapper proc
	jmp mProcs[8*8]
D2D1MakeSkewMatrix_wrapper endp
D2D1SinCos_wrapper proc
	jmp mProcs[9*8]
D2D1SinCos_wrapper endp
D2D1Tan_wrapper proc
	jmp mProcs[10*8]
D2D1Tan_wrapper endp
D2D1Vec3Length_wrapper proc
	jmp mProcs[11*8]
D2D1Vec3Length_wrapper endp
end
