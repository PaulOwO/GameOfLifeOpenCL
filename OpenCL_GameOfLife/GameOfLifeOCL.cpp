#include <array>
#include <iostream>
#define NOMINMAX
#include <Windows.h>
#define __CL_ENABLE_EXCEPTION
#include <fstream>
#include <CL/cl2.hpp>

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;

using array = std::array<std::array<char, 20>, 20>;

array InitialState()
{
	array initialState{};
	for (auto x = 0; x < 20; x++)
	{
		for (auto y = 0; y < 20; y++)
		{
			initialState[x][y] = static_cast<char>(rand() % 2 + 48);
		}
	}
	return initialState;
}


std::ostream& operator << (std::ostream& os, const array& arr)
{
	for (auto x = 0; x < 20; x++)
	{
		for (auto y = 0; y < 20; y++)
		{
			if (arr[x][y] == '1')
			{
				os << " @ ";
			}
			else
			{
				os << " . ";
			}
		}
		os << std::endl;
	}
	return os;
}

const unsigned int platform_id = 0;
const unsigned int device_id = 0;

std::vector<char> CreateVector (const array& state)
{
	std::vector<char> result;
	result.reserve(state.size() * state.size());
	for (int x = 0; x < state.size(); x++)
	{
		for (int y = 0; y < state.size(); y++)
		{
			result.push_back(state[x][y]);
		}
	}
	return result;
}

array CreateArray(const std::vector<char>& state)
{
	array arr;
	int pos = 0;
	for (const auto& elem : state)
	{
		arr[pos % 20][pos / 20] = elem;
		pos++;
	}
	return arr;
}

int main()
{
	// get the device (here the GPU)
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	std::cout << "platform size  : " << platforms.size() << std::endl;
	for (auto& platform : platforms)
	{
		std::cout
			<< "using platform : "
			<< platform.getInfo<CL_PLATFORM_NAME>()
			<< std::endl;
		std::vector<cl::Device> devices_;
		platform.getDevices(CL_DEVICE_TYPE_ALL, &devices_);
		std::cout << "device size    : " << devices_.size() << std::endl;
		for (auto& device : devices_)
		{
			std::cout
				<< "using device   : "
				<< device.getInfo<CL_DEVICE_NAME>()
				<< std::endl;
			cl_context_properties properties[] = {
				CL_CONTEXT_PLATFORM,
				(cl_context_properties)(platform)(),
				0
			};
			cl::Context context_ =
				cl::Context(CL_DEVICE_TYPE_ALL, properties);
			cl::CommandQueue queue_(context_, device, 0, nullptr);
			std::ifstream ifs("./KernelCode.cl");
			if (!ifs.is_open())
			{
				std::cerr << "couldn't load kernelCode\n";
				return -1;
			}
			std::string kernel_source(std::istreambuf_iterator<char>(ifs), {});
			// compile
			cl::Program::Sources source(
				1,
				kernel_source);
			cl::Program program_(context_, source);
			program_.build(devices_);
			// create the kernel
			cl::Kernel kernel_(program_, "simple");
			std::vector<char> in1 = CreateVector(InitialState());
			// prepare the buffers
			cl::Buffer buf_in1_ = cl::Buffer(
				context_,
				CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
				sizeof(char) * in1.size(),
				in1.data());

			std::vector<char> out;
			out.resize(20 * 20);
			
			cl::Buffer buf_out_ = cl::Buffer(
				context_,
				CL_MEM_WRITE_ONLY,
				sizeof(char) * out.size());
			// set the arguments
			kernel_.setArg(0, buf_in1_);
			kernel_.setArg(1, buf_out_);
			// wait to the command queue to finish before proceeding
			queue_.finish();
			auto start = system_clock::now();
			// run the kernel
			queue_.enqueueNDRangeKernel(
				kernel_,
				cl::NullRange,
				cl::NDRange(20 * 20),
				cl::NullRange);
			queue_.finish();
			auto end = system_clock::now();
			// get the result out
			queue_.enqueueReadBuffer(
				buf_out_,
				CL_TRUE,
				0,
				20 * 20 * sizeof(char),
				&out[0]);
			queue_.finish();
			std::cout << "Computing time : ";
			std::cout << duration_cast<microseconds>(end - start).count();
			std::cout << "us" << std::endl;
			std::cout << CreateArray(out);
		}
	}
}