//--------------------------------------------------------------------------------------
// File: test.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this
// file except in compliance with the License.  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED WARRANTIES OR
// CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
//
// See the Apache Version 2.0 License for specific language governing permissions
// and limitations under the License.
//
//--------------------------------------------------------------------------------------
//
/// <tags>P1</tags>
/// <summary>Verify a manual asynchronous synchronization</summary>

#include <amptest.h>
#include <amptest_main.h>

using namespace Concurrency;
using namespace Concurrency::Test;

runall_result test_main()
{
    accelerator acc = require_device();
	
    if(acc.get_supports_cpu_shared_memory())
    {
        acc.set_default_cpu_access_type(ACCESS_TYPE);
    }
	
    accelerator::set_default(acc.get_device_path());

    std::vector<int> v(10);
    std::fill(v.begin(), v.end(), 5);
	
    array_view<int, 1> av(10, v);
    Log() << "Writing on the GPU" << std::endl;
	
    parallel_for_each(extent<1>(1), [=](index<1>) restrict(amp) {
        av[0] = 17;
    });

    Log() << "Forcing a synch" << std::endl;
    std::shared_future<void> w = av.synchronize_async();
    w.wait();
	
    Log() << "Result is: " << v[0] << " Expected: 17" << std::endl;
    return REPORT_RESULT(v[0] == 17);
}
