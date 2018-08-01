#include "dmc.hpp"
#include <nan.h>

#include <fstream>

template <class Scalar>
struct test_object : dmc::object<Scalar>
{
public:
	typedef dmc::object<Scalar> base_type;

	using typename base_type::scalar_type;
	using typename base_type::vector_type;

	explicit test_object(scalar_type radius)
			: radius_(radius)
	{
	}

	virtual scalar_type value(const vector_type &p) const override
	{
		auto abs_p = p.map([](auto x) { return std::abs(x); });

		if (abs_p.x() < abs_p.y())
		{
			if (abs_p.y() < abs_p.z())
				return radius_ - abs_p.z();
			else
				return radius_ - abs_p.y();
		}
		else
		{
			if (abs_p.x() < abs_p.z())
				return radius_ - abs_p.z();
			else
				return radius_ - abs_p.x();
		}
	}

	virtual vector_type grad(const vector_type &p) const override
	{
		auto eps = 1.0e-6;

		return vector_type(
							 value(p + vector_type(eps, 0.0, 0.0)) - value(p - vector_type(eps, 0.0, 0.0)),
							 value(p + vector_type(0.0, eps, 0.0)) - value(p - vector_type(0.0, eps, 0.0)),
							 value(p + vector_type(0.0, 0.0, eps)) - value(p - vector_type(0.0, 0.0, eps))) /
					 (2.0 * eps);
	}

private:
	scalar_type radius_;
};

void GetTestTriangles(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
	dmc::tree<double> t({-3.0, -3.0, -3.0}, {3.0, 3.0, 3.0});
	t.generate(test_object<double>(1.5f));

	std::vector<dmc::triangle3d> triangles;

	t.enumerate([&](const auto &t) {
		triangles.push_back(t);
	});

	v8::Local<v8::Object> jsonObject = Nan::New<v8::Object>();

	v8::Local<v8::String> triProp = Nan::New("triangles").ToLocalChecked();

	v8::Local<v8::Array> triValue = Nan::New<v8::Array>(triangles.size());
	for (int i = 0; i < triValue->Length(); i++)
	{
		v8::Local<v8::Array> points = Nan::New<v8::Array>(3);
		v8::Local<v8::Array> point1 = Nan::New<v8::Array>(3);
		v8::Local<v8::Array> point2 = Nan::New<v8::Array>(3);
		v8::Local<v8::Array> point3 = Nan::New<v8::Array>(3);

		auto triangle = triangles.at(i);
		auto p1 = triangle.p1().template cast<float>();
		auto p2 = triangle.p2().template cast<float>();
		auto p3 = triangle.p3().template cast<float>();

		point1->Set(0, Nan::New(p1.x()));
		point1->Set(1, Nan::New(p1.y()));
		point1->Set(2, Nan::New(p1.z()));

		point2->Set(0, Nan::New(p2.x()));
		point2->Set(1, Nan::New(p2.y()));
		point2->Set(2, Nan::New(p2.z()));

		point3->Set(0, Nan::New(p3.x()));
		point3->Set(1, Nan::New(p3.y()));
		point3->Set(2, Nan::New(p3.z()));

		points->Set(0, point1);
		points->Set(1, point2);
		points->Set(2, point3);
		triValue->Set(i, points);
	}

	Nan::Set(jsonObject, triProp, triValue);

	// std::ofstream os("a.stl", std::ios::binary);
	// write_stl(os, triangles);

	info.GetReturnValue().Set(jsonObject);
}

void Init(v8::Local<v8::Object> exports)
{
	exports->Set(Nan::New("getTestTriangles").ToLocalChecked(),
							 Nan::New<v8::FunctionTemplate>(GetTestTriangles)->GetFunction());
}

NODE_MODULE(hello, Init)