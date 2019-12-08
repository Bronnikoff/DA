#include "BWT.hpp"

using namespace std;



BWT::BWT() : Compressor(){}

BWT::BWT(istream& inpt, ostream& otpt) : Compressor(inpt, otpt){}

BWT::BWT(istream* inpt, ostream* otpt) : Compressor(inpt, otpt){}

// O(nlogn) construct from https://e-maxx.ru/algo/suffix_array
// it will array of sorted cyclics - GREAT

uint32_t BWT::count_karkainen_sanders(vector<uint32_t>& array){
    
}

uint32_t BWT::count_suff_array(vector<uint32_t>& array){
    uint32_t n = in_buffer.size();
    array.resize(n);
    const uint32_t alphabet = 256;

    // equalent classes 
    vector<uint32_t> c(n);
    uint32_t classes = 0;

    // help vectors
    vector<uint32_t> pn(n), cn(n);

    // pn - sorted positions of second parts of new substrings
    // cn - new classes of equality

    // we coding in_buffer fully
    vector<uint32_t> cnt(alphabet, 0);
    // 0 phase - sort chars in string with count sort
    for(uint32_t i = 0; i < n; ++i){
        ++cnt[static_cast<uint8_t>(in_buffer[i])];
    }
    for(uint32_t i = 1; i < alphabet; ++i){
        cnt[i] += cnt[i - 1];
    }
    for(uint32_t i = 0; i < n; ++i){
        array[--cnt[static_cast<uint8_t>(in_buffer[i])]] = i;
    }
    c[array[0]] = 0;
    
    for(uint32_t i = 1; i < n; ++i){
        if(in_buffer[array[i]] != in_buffer[array[i-1]]){
            ++classes;
        }
        c[array[i]] = classes;
    }
    ++classes;

    // all another k phases (k = h + 1)

    for(uint32_t h = 0; (1u << h) < n; ++h){
        // pn - includes old sort positions for string with lenght 2^(k-1)
        // now we should get new sort positions for string with lenght 2^(k)
        // for this we can do radix sort for strings S_k[i] = (S_(k-1)[i], S_(k-1)[i+2^(k-1)])
        // but sort of strings S_k[i] by second positions we can get as pn[i] = p[i] - 2^(k-1) 
        for(uint32_t i = 0; i < n; ++i){
            pn[i] = array[i] < (1u << h) ? array[i] + (n - (1 << h)) : array[i] - (1 << h);
        }

        cnt.assign(classes, 0);

        // and count sort by first positions
        for(uint32_t i = 0; i < n; ++i){
            ++cnt[c[pn[i]]];
        }
        for(uint32_t i = 1; i < classes; ++i){
            cnt[i] += cnt[i - 1];
        }
        for(uint32_t i = n - 1; i > 0; --i){
            array[--cnt[c[pn[i]]]] = pn[i];
        }
        array[--cnt[c[pn[0]]]] = pn[0];


        cn[array[0]] = 0;
        classes = 0;

        // define classes of equlity
        for(uint32_t i = 1; i < n; ++i){
            // mid - second position of new strings
            uint32_t mid1 = (array[i] + (1u << h)) >= n ? 
                array[i] + (1u << h) - n : 
                array[i] + (1u << h); 
            uint32_t mid2 = array[i - 1] + (1u << h) >= n ? 
                array[i - 1] + (1u << h) - n : 
                array[i - 1] + (1u << h);

            if(c[array[i]] != c[array[i - 1]] || c[mid1] != c[mid2]){
                ++classes;
            }
            cn[array[i]] = classes;
        }
        ++classes;
        c = cn;
    }
    return c[0];
}

void BWT::buffer_encode(){
    vector<uint32_t> suf_array;
    position = count_suff_array(suf_array);
    for(uint32_t i = 0; i < in_buffer.size(); ++i){
        if(suf_array[i] > 0){
            out_buffer.push_back(in_buffer[suf_array[i] - 1]);
        }else{
            out_buffer.push_back(in_buffer[in_buffer.size() - 1]);
        }
    }

    uint32_t pos = position;
    uint32_t s_mask = 1 << 8; --s_mask; // 00000000000000000000000011111111

    uint8_t byte4 = static_cast<uint8_t>(pos & s_mask);
    pos >>= 8;
    uint8_t byte3 = static_cast<uint8_t>(pos & s_mask);
    pos >>= 8;
    uint8_t byte2 = static_cast<uint8_t>(pos & s_mask);
    pos >>= 8;
    uint8_t byte1 = static_cast<uint8_t>(pos & s_mask);

    out_buffer.push_back(static_cast<char>(byte1));
    out_buffer.push_back(static_cast<char>(byte2));
    out_buffer.push_back(static_cast<char>(byte3));
    out_buffer.push_back(static_cast<char>(byte4));
}

// TO DO:
// change algo for stay in_buffer without changes

void BWT::buffer_decode(){
    // Reading position number from end of file and pop this number for stay in_buffer as string for decode
    position = 0;
    for(uint8_t i = 0; i < sizeof(uint32_t); ++i){
        uint8_t byte = static_cast<uint8_t>(in_buffer[in_buffer.size() - 1 - i]);
        position |= (static_cast<uint32_t>(byte) << (i * 8));
    }

    // DECODE:
    // this algo i get from https://neerc.ifmo.ru/wiki/index.php?title=Преобразование_Барроуза-Уилера
    const uint32_t alphabet = 256;
    const uint32_t n = in_buffer.size() - sizeof(uint32_t);
    vector<uint32_t> count(alphabet, 0);
    vector<uint32_t> t(n);

    // count sort in_buffer and save positions in t

    for(uint32_t i = 0; i < n; ++i){
        ++count[static_cast<uint8_t>(in_buffer[i])];
    }

    uint32_t sum = 0;
    for(uint32_t i = 0; i < alphabet; ++i){
        sum += count[i];
        count[i] = sum - count[i];
    }

    for(uint32_t i = 0; i < n; ++i){
        t[count[static_cast<uint8_t>(in_buffer[i])]++] = i;
    }

    // decode in_buffer using t in out_buffer
    out_buffer.resize(n);
    uint32_t j = t[position];
    for(uint32_t i = 0; i < n; ++i){
        out_buffer[i] = in_buffer[j];
        j = t[j];
    }
}

