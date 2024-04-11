#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <utility>

using namespace std::literals;

class Domain {
public:
    Domain() = default;

    template <typename String>
    Domain(String&& name)
        : reversed_name_(std::forward<String>(name)) {
            std::reverse(reversed_name_.begin(), reversed_name_.end());
            reversed_name_ += "."s;
    }
    
    bool operator==(const Domain& other) const {
        return reversed_name_ == other.reversed_name_;
    }

    bool operator<(const Domain& other) const {
        return reversed_name_ < other.reversed_name_;
    }

    bool operator>(const Domain& other) const {
        return other < *this;
    }

    size_t size() const {
        return reversed_name_.size();
    }

    std::string GetName() const {
        return {reversed_name_.rbegin() + 1, reversed_name_.rend()};
    }

    std::string_view GetReversedName() const {
        return reversed_name_;
    }

    bool IsSubdomain(const Domain& main_domain) const {
        if (*this < main_domain) {
            return false;
        }

        auto main_domain_name = main_domain.GetReversedName();
        if (std::equal(main_domain_name.begin(), main_domain_name.end(),
                       reversed_name_.begin())) {
            return true;
        }
        return false;
    }

private:
    std::string reversed_name_;
};

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён

    template <typename InputIt>
    DomainChecker(InputIt first, InputIt last)
        : forbidden_domains_(first, last) {
            std::sort(forbidden_domains_.begin(), forbidden_domains_.end());

            auto new_last = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(),
                                        [](const Domain& lhs, const Domain& rhs) {
                return rhs.IsSubdomain(lhs);
            });

            forbidden_domains_.erase(new_last, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) {
        auto upper_iter = std::upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);
        if (upper_iter == forbidden_domains_.begin()) {
            return false;
        }

        return domain.IsSubdomain(*(upper_iter - 1));
    }

private:
    std::vector<Domain> forbidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа

template <typename Number>
std::vector<Domain> ReadDomains(std::istream& input, Number size) {
    std::vector<Domain> domains;
    for (; size > 0; --size) {
        std::string line;
        std::getline(input, line);
        domains.push_back(Domain{std::move(line)});
    }
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(std::istream& input) {
    std::string line;
    std::getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}
/* Тестовые кейсы из треда в Пачке, реализованные через файловые потоки и построчную проверку ответов

void TestDomain() {
    Domain test_domain1("gdz.ru"s);
    assert(test_domain1.GetName() == "gdz.ru"s);
    assert(test_domain1.GetReversedName() == "ur.zdg."s);
    assert(test_domain1.size() == 7);

    Domain test_domain2("m.gdz.ru"s);
    assert(test_domain1 < test_domain2);
    assert(test_domain2 > test_domain1);
    assert(test_domain2.IsSubdomain(test_domain1));

    Domain test_domain1_equal("gdz.ru"s);
    assert(test_domain1 == test_domain1_equal);
    assert(test_domain1.IsSubdomain(test_domain1_equal));

    Domain test_domain3("freegdz.ru"s);
    assert(!test_domain3.IsSubdomain(test_domain1));
}

void TestCases() {
    std::string file_name1 = "test1_input.txt"s;
    std::fstream answers1("test1_output.txt"s);
    std::string file_name2 = "test2_input.txt"s;
    std::fstream answers2("test2_output.txt"s);

    {
        std::fstream case1(file_name1);
        const std::vector<Domain> forbidden_domains = ReadDomains(case1, ReadNumberOnLine<size_t>(case1));
        DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

        const std::vector<Domain> test_domains = ReadDomains(case1, ReadNumberOnLine<size_t>(case1));
        
        std::fstream output1("answers1.txt"s);
        for (const Domain& domain : test_domains) {
            output1 << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << std::endl;
        }

        std::string right_answer;
        std::string my_answer;
        int line_count = 1;
        while (std::getline(answers1, right_answer) && std::getline(output1, my_answer)) {
            std::cerr << "Line " << line_count << std::endl;
            assert(my_answer == right_answer);
            ++line_count;
        }
    }

    {
        std::fstream case2(file_name2);
        const std::vector<Domain> forbidden_domains = ReadDomains(case2, ReadNumberOnLine<size_t>(case2));
        DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

        const std::vector<Domain> test_domains = ReadDomains(case2, ReadNumberOnLine<size_t>(case2));

        std::fstream output2("answers2.txt"s);
        for (const Domain& domain : test_domains) {
            output2 << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << std::endl;
        }

        std::string right_answer;
        std::string my_answer;
        int line_count = 0;
        while (std::getline(answers2, right_answer) && std::getline(output2, my_answer)) {
            std::cerr << "Line " << line_count << std::endl;
            assert(my_answer == right_answer);
            ++line_count;
        }
    }
}

*/

int main() {
    // TestDomain();
    // TestCases();
    const std::vector<Domain> forbidden_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    for (const Domain& domain : test_domains) {
        std::cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << std::endl;
    }
}
