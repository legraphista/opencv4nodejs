#include "io.h"
#include "Mat.h"
#include "VideoCapture.h"
#include <iostream>

NAN_MODULE_INIT(Io::Init) {
  Nan::SetMethod(target, "imread", Imread);
  Nan::SetMethod(target, "imshow", Imshow);
	Nan::SetMethod(target, "imshowWait", ImshowWait);
  Nan::SetMethod(target, "imwrite", Imwrite);
	Nan::SetMethod(target, "waitKey", WaitKey);
	Nan::SetMethod(target, "imencode", Imencode);
	Nan::SetMethod(target, "imdecode", Imdecode);

	FF_SET_JS_PROP(target, IMREAD_UNCHANGED, Nan::New<v8::Integer>(cv::IMREAD_UNCHANGED));
	FF_SET_JS_PROP(target, IMREAD_GRAYSCALE, Nan::New<v8::Integer>(cv::IMREAD_GRAYSCALE));
	FF_SET_JS_PROP(target, IMREAD_COLOR, Nan::New<v8::Integer>(cv::IMREAD_COLOR));
	FF_SET_JS_PROP(target, IMREAD_ANYDEPTH, Nan::New<v8::Integer>(cv::IMREAD_ANYDEPTH));
	FF_SET_JS_PROP(target, IMREAD_ANYCOLOR, Nan::New<v8::Integer>(cv::IMREAD_ANYCOLOR));
	FF_SET_JS_PROP(target, IMREAD_LOAD_GDAL, Nan::New<v8::Integer>(cv::IMREAD_LOAD_GDAL));

	FF_SET_JS_PROP(target, IMWRITE_JPEG_QUALITY, Nan::New<v8::Integer>(cv::IMWRITE_JPEG_QUALITY));
	FF_SET_JS_PROP(target, IMWRITE_JPEG_PROGRESSIVE, Nan::New<v8::Integer>(cv::IMWRITE_JPEG_PROGRESSIVE));
	FF_SET_JS_PROP(target, IMWRITE_JPEG_OPTIMIZE, Nan::New<v8::Integer>(cv::IMWRITE_JPEG_OPTIMIZE));
	FF_SET_JS_PROP(target, IMWRITE_JPEG_RST_INTERVAL, Nan::New<v8::Integer>(cv::IMWRITE_JPEG_RST_INTERVAL));
	FF_SET_JS_PROP(target, IMWRITE_JPEG_LUMA_QUALITY, Nan::New<v8::Integer>(cv::IMWRITE_JPEG_LUMA_QUALITY));
	FF_SET_JS_PROP(target, IMWRITE_JPEG_CHROMA_QUALITY, Nan::New<v8::Integer>(cv::IMWRITE_JPEG_CHROMA_QUALITY));
	FF_SET_JS_PROP(target, IMWRITE_PNG_COMPRESSION, Nan::New<v8::Integer>(cv::IMWRITE_PNG_COMPRESSION));
	FF_SET_JS_PROP(target, IMWRITE_PNG_STRATEGY, Nan::New<v8::Integer>(cv::IMWRITE_PNG_STRATEGY));
	FF_SET_JS_PROP(target, IMWRITE_PNG_BILEVEL, Nan::New<v8::Integer>(cv::IMWRITE_PNG_BILEVEL));
	FF_SET_JS_PROP(target, IMWRITE_PXM_BINARY, Nan::New<v8::Integer>(cv::IMWRITE_PXM_BINARY));
	FF_SET_JS_PROP(target, IMWRITE_WEBP_QUALITY, Nan::New<v8::Integer>(cv::IMWRITE_WEBP_QUALITY));

#if OPENCV_MINOR_VERSION >= 1
	FF_SET_JS_PROP(target, IMREAD_REDUCED_GRAYSCALE_2, Nan::New<v8::Integer>(cv::IMREAD_REDUCED_GRAYSCALE_2));
	FF_SET_JS_PROP(target, IMREAD_REDUCED_COLOR_2, Nan::New<v8::Integer>(cv::IMREAD_REDUCED_COLOR_2));
	FF_SET_JS_PROP(target, IMREAD_REDUCED_GRAYSCALE_4, Nan::New<v8::Integer>(cv::IMREAD_REDUCED_GRAYSCALE_4));
	FF_SET_JS_PROP(target, IMREAD_REDUCED_COLOR_4, Nan::New<v8::Integer>(cv::IMREAD_REDUCED_COLOR_4));
	FF_SET_JS_PROP(target, IMREAD_REDUCED_GRAYSCALE_8, Nan::New<v8::Integer>(cv::IMREAD_REDUCED_GRAYSCALE_8));
	FF_SET_JS_PROP(target, IMREAD_REDUCED_COLOR_8, Nan::New<v8::Integer>(cv::IMREAD_REDUCED_COLOR_8));
#endif
#if OPENCV_MINOR_VERSION >= 2
	FF_SET_JS_PROP(target, IMREAD_IGNORE_ORIENTATION, Nan::New<v8::Integer>(cv::IMREAD_IGNORE_ORIENTATION));
	FF_SET_JS_PROP(target, IMWRITE_PAM_TUPLETYPE, Nan::New<v8::Integer>(cv::IMWRITE_PAM_TUPLETYPE));

	FF_SET_JS_PROP(target, IMWRITE_PAM_FORMAT_NULL, Nan::New<v8::Integer>(cv::IMWRITE_PAM_FORMAT_NULL));
	FF_SET_JS_PROP(target, IMWRITE_PAM_FORMAT_BLACKANDWHITE, Nan::New<v8::Integer>(cv::IMWRITE_PAM_FORMAT_BLACKANDWHITE));
	FF_SET_JS_PROP(target, IMWRITE_PAM_FORMAT_GRAYSCALE, Nan::New<v8::Integer>(cv::IMWRITE_PAM_FORMAT_GRAYSCALE));
	FF_SET_JS_PROP(target, IMWRITE_PAM_FORMAT_GRAYSCALE_ALPHA, Nan::New<v8::Integer>(cv::IMWRITE_PAM_FORMAT_GRAYSCALE_ALPHA));
	FF_SET_JS_PROP(target, IMWRITE_PAM_FORMAT_RGB, Nan::New<v8::Integer>(cv::IMWRITE_PAM_FORMAT_RGB));
	FF_SET_JS_PROP(target, IMWRITE_PAM_FORMAT_RGB_ALPHA, Nan::New<v8::Integer>(cv::IMWRITE_PAM_FORMAT_RGB_ALPHA));
#endif
	FF_SET_JS_PROP(target, IMWRITE_PNG_STRATEGY_DEFAULT, Nan::New<v8::Integer>(cv::IMWRITE_PNG_STRATEGY_DEFAULT));
	FF_SET_JS_PROP(target, IMWRITE_PNG_STRATEGY_FILTERED, Nan::New<v8::Integer>(cv::IMWRITE_PNG_STRATEGY_FILTERED));
	FF_SET_JS_PROP(target, IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY, Nan::New<v8::Integer>(cv::IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY));
	FF_SET_JS_PROP(target, IMWRITE_PNG_STRATEGY_RLE, Nan::New<v8::Integer>(cv::IMWRITE_PNG_STRATEGY_RLE));
	FF_SET_JS_PROP(target, IMWRITE_PNG_STRATEGY_FIXED, Nan::New<v8::Integer>(cv::IMWRITE_PNG_STRATEGY_FIXED));

	VideoCapture::Init(target);
};

NAN_METHOD(Io::Imread) {
	FF_METHOD_CONTEXT("Imread");
  if (!info[0]->IsString()) {
		FF_THROW("expected arg0 to be path");
  }

  std::string path = FF_CAST_STRING(info[0]);
	cv::Mat mat = cv::imread(path);
	if (mat.rows == 0 && mat.cols == 0) {
		FF_THROW("empty mat");
	}
	FF_OBJ jsMat = FF_NEW_INSTANCE(Mat::constructor);
	FF_UNWRAP_MAT_AND_GET(jsMat) = mat;
	FF_RETURN(jsMat);
}

NAN_METHOD(Io::Imwrite) {
	FF_METHOD_CONTEXT("Imwrite");
  if (!info[0]->IsString()) {
		FF_THROW("expected arg0 to be path");
	}
	if (!FF_IS_INSTANCE(Mat::constructor, info[1])) {
		FF_THROW("expected arg1 to be an instance of Mat");
	}
	cv::imwrite(FF_CAST_STRING(info[0]), FF_UNWRAP_MAT_AND_GET(info[1]->ToObject()));
}

NAN_METHOD(Io::Imshow) {
	FF_METHOD_CONTEXT("Imshow");
  if (!info[0]->IsString()) {
		FF_THROW("expected arg0 to be the window name");
  }
	if (!FF_IS_INSTANCE(Mat::constructor, info[1])) {
		FF_THROW("expected arg1 to be an instance of Mat");
	}
	cv::imshow(FF_CAST_STRING(info[0]), FF_UNWRAP_MAT_AND_GET(info[1]->ToObject()));
}

NAN_METHOD(Io::ImshowWait) {
	FF_METHOD_CONTEXT("ImshowWait");
	if (!info[0]->IsString()) {
		FF_THROW("expected arg0 to be the window name");
	}
	if (!FF_IS_INSTANCE(Mat::constructor, info[1])) {
		FF_THROW("expected arg1 to be an instance of Mat");
	}
	cv::imshow(FF_CAST_STRING(info[0]), FF_UNWRAP_MAT_AND_GET(info[1]->ToObject()));
	cv::waitKey();
}

NAN_METHOD(Io::WaitKey) {
	int key;
	if (info[0]->IsNumber()) {
		key = cv::waitKey(info[0]->NumberValue());
	} else{
		key = cv::waitKey();
	}
	FF_RETURN(Nan::New(key));
}

NAN_METHOD(Io::Imencode) {
	FF_METHOD_CONTEXT("Io::Imencode");
	FF_ARG_STRING(0, std::string ext);
	FF_ARG_INSTANCE(1, cv::Mat img, Mat::constructor, FF_UNWRAP_MAT_AND_GET);
	FF_ARG_UNPACK_INT_ARRAY_IFDEF(2, flags, std::vector<int>());

	std::vector<uchar> data;
	cv::imencode(ext, img, data, flags);

	size_t size = data.size() * sizeof(char);
	char *ndata = static_cast<char *>(malloc(size));
	memcpy(ndata, reinterpret_cast<char*>(data.data()), size);

	FF_RETURN(Nan::NewBuffer(ndata, size).ToLocalChecked());
}

NAN_METHOD(Io::Imdecode) {
	FF_METHOD_CONTEXT("Io::Imencode");

	if (!info[0]->IsUint8Array()) {
		FF_THROW("expected arg 0 to be a Buffer of Uint8 Values");
	}
	FF_ARG_INT_IFDEF(1, int flags, cv::IMREAD_ANYCOLOR);

	char *data = static_cast<char *>(node::Buffer::Data(info[0]->ToObject()));
	size_t size = node::Buffer::Length(info[0]->ToObject());
	std::vector<uchar> vec(size);
	memcpy(vec.data(), data, size);

	FF_OBJ jsDecodedMat = FF_NEW_INSTANCE(Mat::constructor);
	FF_UNWRAP_MAT_AND_GET(jsDecodedMat) = cv::imdecode(vec, flags);
	FF_RETURN(jsDecodedMat);
}