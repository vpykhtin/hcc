// Copyright (c) Microsoft
// All rights reserved
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
// INCLUDING WITHOUT LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
// See the Apache Version 2.0 License for specific language governing permissions and limitations under the License.
/// <tags>P1</tags>
/// <summary>Array(extent based) constructed using just begin iterator. Iterator has more data than extent - use vector</summary>

#include <map>
#include "./../../../constructor.h"
#include <amptest_main.h>

template<typename _type, int _rank, typename _src_data, typename _BeginIterator>
bool test_feature_itr_local(extent<_rank> _e, _BeginIterator _first, _src_data _data)
{
    // Init array
    array<_type, _rank> src(_e, _first);
    array<_type, _rank> dst(_e);
    parallel_for_each(src.get_extent(), [&] (index<_rank> idx) __GPU_ONLY
    {
        dst[idx] = src(idx);
    });

    std::vector<_type> src_data = src;
    std::vector<_type> dst_data = dst;

    if ((src_data.size() != _e.size()) || (dst_data.size() != _e.size()))
    {
        std::cout << "Invalid size : src - " << src_data.size() << " dst - " << dst_data.size() << std::endl;
        return false;
    }

    return Verify(dst_data, src_data);
}

template<typename _type, int _rank>
bool test_feature()
{
    int edata[_rank];
    for (int i = 0; i < _rank; i++)
        edata[i] = i+1;
    extent<_rank> e1(edata);

    std::vector<_type> data(e1.size()+1);
    for (unsigned int i = 0; i < e1.size()+1; i++)
        data[i] = (_type)rand();

    {
        if (test_feature_itr_local<_type, _rank>(e1, data.begin(), data) == false)
            return false;
    }

    return true;
}

runall_result test_main()
{
	accelerator::set_default(require_device().get_device_path());

	runall_result result;

	result &= REPORT_RESULT((test_feature<int, 5>()));

	return result;
}

