#pragma once
#include <cstdint>
#include <iostream>
#include <cmath>
#include <bitset>

namespace clib
{
     /*!
        \~english
            \brief
                A structure.

            \details
                Just a demo of documentation.

        \~russian
            \brief
                Структура.

            \details
                Для демонстрации автодокументирования.

        \~  \see myfunc
     */
    template <
        uint32_t BIAS, 
        uint32_t EXP_WIDTH, 
        uint32_t MANT_WIDTH
    >
    struct fp final
    {
    private:
        bool is_nan;
        uint32_t sign_;
        uint32_t exp_;
        uint32_t mant_;

        bool is_valid() const
        {
            if (sign_ > 1)
                return false;
            if (exp_ >= (1 << EXP_WIDTH))
                return false;
            if (mant_ >= (1 << MANT_WIDTH))
                return false;
            return true;
        }

        void set_nan()
        {
            is_nan = true;
            sign_ = 0;
            exp_ = (1 << EXP_WIDTH) - 1;
            mant_ = 1;
        }

        bool is_valid_exp(uint32_t exp) const
        {
            if (exp >= (1 << EXP_WIDTH) - 1 + BIAS)
                return false;
            if (exp <= BIAS)
                return false;
            return true;
        }

        bool is_float() const
        {
            if (BIAS == 127 && EXP_WIDTH == 8 && MANT_WIDTH == 23)
                return true;
            return false;
        }

    public:
        fp(uint32_t sign_, uint32_t exp_, uint32_t mant_)
            : is_nan(false)
            , sign_(sign_)
            , exp_(exp_)
            , mant_(mant_)
        {
            if (!is_valid())
                set_nan();
        }

        explicit fp (float fin)
        {
            uint32_t fint = *reinterpret_cast<uint32_t*>(&fin);

            sign_ = (fint << 0) >> 31;
            exp_  = (fint << 1) >> 24;
            mant_ = (fint << 9) >> 9;

            is_nan = is_valid();
        }


        fp operator* (const fp& right) const
        {
            if (!is_valid() || !right.is_valid())
            {
                std::cerr << "Error. Invalid args in *\n";
                return get_nan();
            }

            uint32_t nsign = sign_ ^ right.sign_;
            uint32_t nexp = exp_ + right.exp_ - BIAS;

            if (!is_valid_exp(nexp))
                return get_nan();

            //print_bit_interpr();   
            //right.print_bit_interpr();

            uint32_t nmant = mant_ + right.mant_ +
                static_cast<uint32_t>(static_cast<uint64_t>(mant_) * right.mant_ >> MANT_WIDTH);

            while (nmant >= (1 << MANT_WIDTH))
            {
                nexp++;
                if (!is_valid_exp(nexp))
                    return get_nan();
                
                nmant -= (1 << MANT_WIDTH);
                nmant >>= 1;
            }

            //fp(nsign, nexp, nmant).print_bit_interpr();
            return fp(nsign, nexp, nmant);
        }

        float get_float() const
        {
            if (is_float())
            {
                uint32_t float_as_int = 0;
    
                float_as_int |= (sign_ << 31);
                float_as_int |= (exp_  << 23);
                float_as_int |= (mant_ << 0);
    
                return *reinterpret_cast<float*>(&float_as_int);
            }
            return nan("");
        }

        void print_bit_interpr() const
        {
            if (sign_)
                std::cout << 1 << " ";
            else
                std::cout << 0 << " ";
            
            std::bitset<EXP_WIDTH> exp(exp_);
            std::bitset<MANT_WIDTH> mant(mant_);

            std::cout << exp << " " << mant << std::endl;
        }

        static fp get_nan()
        {
            return fp(2, 0, 0);
        }
    };

    using fp32 = fp<127, 8, 23>;
}
