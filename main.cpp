#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

static std::string NowUtcIso8601()
{
    using namespace std::chrono;

    const auto now = system_clock::now();
    const auto t = system_clock::to_time_t(now);

    std::tm tmUtc{};
#if defined(_WIN32)
    gmtime_s(&tmUtc, &t);
#else
    gmtime_r(&t, &tmUtc);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tmUtc, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

static int SumUpTo(int n)
{
    if (n <= 0)
    {
        return 0;
    }

    int sum = 0;
    for (int i = 1; i <= n; ++i)
    {
        sum += i;
    }
    return sum;
}

int main(int argc, char** argv)
{
    std::cout << "Simple C++17 app running.\n";
    std::cout << "UTC time: " << NowUtcIso8601() << "\n";

    std::string name = "Docker World";
    if (argc >= 2)
    {
        name = argv[1];
    }
    std::cout << "Hello, " << name << "!\n";

    std::vector<int> values{ 3, 1, 4, 1, 5, 9 };
    const int sum = std::accumulate(values.begin(), values.end(), 0);

    std::cout << "Values: ";
    for (size_t i = 0; i < values.size(); ++i)
    {
        std::cout << values[i] << (i + 1 < values.size() ? ", " : "");
    }
    std::cout << "\n";

    std::cout << "Sum(values) = " << sum << "\n";
    std::cout << "Sum(1..10) = " << SumUpTo(10) << "\n";

    return 0;
}

