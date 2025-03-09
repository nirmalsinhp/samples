#include <iostream>
#include <cmath>

class BlackScholes {
public:
    BlackScholes(double S, double K, double T, double r, double sigma)
        : S(S), K(K), T(T), r(r), sigma(sigma) {}

    double optionPrice(char optionType) {
        double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
        double d2 = d1 - sigma * std::sqrt(T);

        if (optionType == 'C') {
            return S * normCDF(d1) - K * std::exp(-r * T) * normCDF(d2);
        } else if (optionType == 'P') {
            return K * std::exp(-r * T) * normCDF(-d2) - S * normCDF(-d1);
        } else {
            throw std::invalid_argument("Invalid option type. Use 'C' for Call or 'P' for Put.");
        }
    }

private:
    double S;     // Underlying asset price
    double K;     // Strike price
    double T;     // Time to maturity
    double r;     // Risk-free rate
    double sigma; // Volatility

    double normCDF(double x) {
        return 0.5 * std::erfc(-x * std::sqrt(0.5));
    }
};

int main() {
    double S = 100.0;   // Underlying asset price
    double K = 100.0;   // Strike price
    double T = 1.0;     // Time to maturity (1 year)
    double r = 0.05;    // Risk-free rate (5%)
    double sigma = 0.2; // Volatility (20%)

// looping from strike price 95 to 100
    for (int i = 95; i <= 100; i++) {
        BlackScholes bs(S, i, T, r, sigma);
        double callPrice = bs.optionPrice('C');
        double putPrice = bs.optionPrice('P');

        std::cout << "Strike Price: " << i << std::endl;
        std::cout << "Call Option Price: " << callPrice << std::endl;
        std::cout << "Put Option Price: " << putPrice << std::endl;
    }
    return 0;
}