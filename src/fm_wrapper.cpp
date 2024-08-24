#include "fm_wrapper.h"
#include <sdsl/bit_vectors.hpp>
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <tuple>
#include <vector>


/**
* @brief FM Wrapper Constructor
*
* Does nothing 
*
*/

FM_Wrapper::FM_Wrapper(){}

/**
* @brief FM Wrapper Destructor
*
* Does nothing 
*
*/

FM_Wrapper::~FM_Wrapper(){}

/**
* @brief Extend backward match with FM-index (LF Mapping)
*
* A wrapper around sdsl::csa_wt<sdsl::wt_huff<sdsl::rrr_vector<127>>, 512, 1024> fm_index.
* Calculates the new backwards search range after trying to match the next char (backwards)
* Can continue from previous character match so do not have to keep redoing previously done backwards matches.
*
* @param[in] fm_index [sdsl::csa_wt<sdsl::wt_huff<sdsl::rrr_vector<127>>, 512, 1024>] the fm_index queried
* @param[in] prev_backward_range [sdsl::range] the previous backwards search range.
* @param[in] next_char [char] the next character to match 
* 
* @return the backwards search range of next_char 
*/

std::tuple<size_t, size_t> FM_Wrapper::backward_match(const sdsl::csa_wt<sdsl::wt_huff<sdsl::rrr_vector<127>>, 512, 1024>& fm_index, 
                                                    const std::tuple<size_t, size_t>& prev_backward_range,
                                                    const char next_char)
{
    std::tuple<size_t, size_t> next_char_backward_range;
    std::size_t next_left = fm_index.bwt.rank(std::get<0>(prev_backward_range), next_char);
    std::size_t next_right = fm_index.bwt.rank(std::get<1>(prev_backward_range), next_char);

    // There is a special (smaller) character appended so have to add 1 to the offset for both
    // For bit 1 have to offset the range by the number of 0 bits. 
    if (next_char == '0'){
        next_left += 1;
        next_right += 1;
    }
    if (next_char == '1'){
        next_left += this->rank0 + 1;
        next_right += this->rank0 + 1;
    }

    return std::make_tuple(next_left,next_right);
}

/**
* @brief Get corresponding location in reference via suffix array
*
* A wrapper around sdsl::csa_wt<sdsl::wt_huff<sdsl::rrr_vector<127>>, 512, 1024> fm_index.
* When the backwards search range becomes empty it means there is no perfect match with the current pattern that is being processed.
* With the prior non-empty range we find one location of the previous perfect pattern match. This should always be next_left of backward_match
*
* @param[in] fm_index [sdsl::csa_wt<sdsl::wt_huff<sdsl::rrr_vector<127>>, 512, 1024>] the fm_index queried
* @param[in] location [size_t] I think should always be next_left of backward_match.
* 
* @return the suffix array index
*/

size_t FM_Wrapper::get_suffix_array_value(const sdsl::csa_wt<sdsl::wt_huff<sdsl::rrr_vector<127>>, 512, 1024>& fm_index,
                                        const size_t location)
{
    return fm_index[location];
}