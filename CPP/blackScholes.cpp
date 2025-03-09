#include <bits/stdc++.h>
using namespace std;


enum class PayOffType
{
    call = 1,
    put = -1
};

class BlackScholes
{
public:
    BlackScholes(double strike, double spot, double time_to_exp,
        PayOffType pt, double rfrate, double divd = 0.0) :
        strike_(strike), spot_(spot), time_to_exp_(time_to_exp), pot(pt), rate_(rfrate), div_(divd)
    {
    }
    double operator()(double vol)
    {
        using std::exp;
        // phi, as in the James book (reference {5}):
        const int phi = static_cast<int>(pot);

        if (time_to_exp_ > 0.0)
        {
            auto norm_args = compute_norm_args_(vol);
            double d1 = norm_args[0];
            double d2 = norm_args[1];

            auto norm_cdf = [](double x)
                {
                    return (1.0 + std::erf(x / std::numbers::sqrt2)) / 2.0;
                };

            double nd_1 = norm_cdf(phi * d1);                 // N(d1) 
            double nd_2 = norm_cdf(phi * d2);                 // N(d2)
            double disc_fctr = exp(-rate_ * time_to_exp_);

            return phi * (spot_ * exp(-div_ * time_to_exp_)
                * nd_1 - disc_fctr * strike_ * nd_2);
        }
        else
        {
            // std::max in <algorithm> (to be covered in Chapter 5)
            return std::max(phi * (spot_ - strike_), 0.0);
        }
    }

private:
    array<double, 2> compute_norm_args(double vol);

    double strike_;
    double spot_;
    double time_to_exp_;
    PayOffType pot;
    double rate_;
    double div_;


};


double implied_volatility_with_lambda(const BlackScholes& bsc,
    double opt_mkt_price, double x0, double x1, double tol,
    unsigned max_iter)                                          
{
    auto diff = [&bsc, opt_mkt_price](double x)
    {
        return bsc(x) - opt_mkt_price;
    };

    double y0 = diff(x0);                                       
    double y1 = diff(x1);

    double impl_vol = 0.0;
    unsigned count_iter = 0;
    for (count_iter = 0; count_iter <= max_iter; ++count_iter)  
    {
        // x0, x1: store previous and updated volatility values
        // through each iteration.

        // y0, y1: store previous and updated values of
        // BSc opt price - opt mkt price.

        if (std::abs(x1 - x0) > tol)
        {
            impl_vol = x1 - (x1 - x0) * y1 / (y1 - y0);

            // Update x1 & x0:
            x0 = x1;
            x1 = impl_vol;
            y0 = y1;

            // Use lambda expression instead of y1 = bsc(x1) - opt_mkt_price:
            y1 = diff(x1);
        }
        else
        {
            return x1;        //  (convergence) 
        }
    }

    return std::nan("");      //  (does not converge) 

}

int main()
{

}