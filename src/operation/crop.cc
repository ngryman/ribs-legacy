/*!
 * ribs
 * Copyright (c) 2013-2014 Nicolas Gryman <ngryman@gmail.com>
 * LGPL Licensed
 */

#include "crop.h"
#include "../image.h"

using namespace v8;
using namespace node;
using namespace ribs;

OPERATION_PREPARE(Crop, {
	// check against mandatory image input (from this)
	image = ObjectWrap::Unwrap<Image>(args.This());

	// create a persistent object during the process to avoid v8 to dispose the JavaScript image object.
	NanAssignPersistent(Object, imageHandle, args.This());

	// store width & height
	width  = args[0]->Uint32Value();
	height = args[1]->Uint32Value();
	x      = args[2]->Uint32Value();
	y      = args[3]->Uint32Value();
})

OPERATION_CLEANUP(Crop, {
	if (!imageHandle.IsEmpty()) NanDisposePersistent(imageHandle);
})

OPERATION_PROCESS(Crop, {
	try {
		cv::Rect roi(x, y, width, height);
		cv::Mat mat = image->Matrix();
		cv::Mat res = mat(roi).clone();

		image->Matrix(res);
	}
	catch (const cv::Exception& e) {
		error = "operation error: crop";
	}
})

OPERATION_VALUE(Crop, {
	image->Sync(imageHandle);
	return NanPersistentToLocal(imageHandle);
})
