
#include <fstream>
#include <cassert>
#include <vector>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vs) {
    out << "[";

    bool first = true;
    for (const auto& v : vs) {
        if (!first) out << ",";
        first = false;
        out << v;
    }

    out << "]";
    return out;
}

namespace {

std::vector<std::string_view> split(std::string_view s, char sep) {
    std::vector<std::string_view> pages;
    while (true) {
        auto pos = s.find(sep);
        pages.push_back(s.substr(0, pos));
        if (pos == s.npos)
            break;
        s.remove_prefix(pos + 1);
    }

    return pages;
}

void test_split() {
    {
        auto pages = split("word", ',');
        assert(pages.size() == 1);
        assert(pages[0] == "word");
    }
    {
        auto pages = split("a,b", ',');
        assert(pages.size() == 2);
        assert(pages[0] == "a");
        assert(pages[1] == "b");
    }
}

std::vector<std::string> readOrderingRules(std::ifstream& inf) {
    std::vector<std::string> rules;
    std::string line;
    while (std::getline(inf, line)) {
        if (line.empty()) {
            // Ordering rules have been read!
            break;
        }
        rules.push_back(std::move(line));
    }
    return rules;
}

using PageView = std::string_view;

enum class PageOrder {
    GreaterThan,
    LowerThan,
    Equal,
    Unspecified,
    Ambiguous,
};

class PageOrderIndex {
public:
    using Index = std::unordered_map<PageView, std::unordered_set<PageView>>;

    PageOrderIndex(std::vector<std::string>&& rules) : rules_(std::move(rules)) {
        for (const auto& rule : rules_) {
            auto pages = split(rule, '|');
            assert(pages.size() == 2);
            lt_index_[pages[0]].emplace(pages[1]);
            gt_index_[pages[1]].emplace(pages[0]);
        }
    }

    PageOrder compare(PageView lhs, PageView rhs) const {
        if (lhs == rhs) 
            return PageOrder::Equal;
        bool is_lt = checkRelation(lhs, rhs, lt_index_);
        bool is_gt = checkRelation(lhs, rhs, gt_index_);
        if (is_lt && is_gt) 
            return PageOrder::Ambiguous;
        if (is_lt)
            return PageOrder::LowerThan;
        if (is_gt)
            return PageOrder::GreaterThan;
        return PageOrder::Unspecified;
    }

    // brief: return all pages that is greater than |pv| w.r.t the ordering rules
    const std::unordered_set<PageView>* gtPageViews(PageView pv) const {
        auto it = lt_index_.find(pv);
        return (it == lt_index_.end()) ? nullptr : &it->second;
    }

    // brief: return all pages that is lower than |pv| w.r.t the ordering rules
    const std::unordered_set<PageView>* ltPageViews(PageView pv) const {
        auto it = gt_index_.find(pv);
        return (it == gt_index_.end()) ? nullptr : &it->second;
    }

private:
    static bool checkRelation(PageView lhs, PageView rhs, const Index& index) {
        auto it = index.find(lhs);
        if (it != index.end()) 
            return false;
        auto jt = it->second.find(rhs);
        return jt != it->second.end();
    }

    std::vector<std::string> rules_; 
    Index gt_index_; // greater-than index
    Index lt_index_; // lower-than index
};



/*
graph:
|V| in pvs
|E| in order_index.ltPageViews

returns pvs topologically sorted
returns empty container if graph contains cycles
*/
std::vector<PageView> topoligical_sort(const std::vector<PageView>& pvs, const PageOrderIndex& order_index) {
    std::vector<PageView> tsorted_pages;

    std::unordered_set<PageView> ps(pvs.begin(), pvs.end());
    auto in_ps = [&ps](PageView pv) { return ps.find(pv) != ps.end(); };

    std::unordered_set<PageView> visited;
    auto is_visited = [&visited](PageView pv) { return visited.find(pv) != visited.end(); };

    std::vector<PageView> to_visit;
    for (auto p : ps) {

        if (is_visited(p)) 
            continue;
        to_visit.push_back(p);

        // non-recursive dfs 
        while (!to_visit.empty()) {
            auto v = to_visit.back();

            if (auto* lts = order_index.ltPageViews(v)) {
                for (auto u : *lts) {
                    
                    if (!is_visited(u)) {
                        if (!in_ps(u)) 
                            continue;
                        
                        // rude cycle checking
                        for (auto w : to_visit) {
                            if (w == u) {
                                std::cerr << "Cycle found!\n";
                                std::cerr << "Pages: " << pvs << std::endl;
                                std::cerr << "to_visit: " << to_visit << std::endl;
                                std::cerr << "new page: " << u << std::endl;
                                return {};
                            }
                        }

                        to_visit.push_back(u);
                        break;
                    }
                }
            }

            if (to_visit.back() == v) {
                tsorted_pages.push_back(v);
                to_visit.pop_back();
                visited.emplace(v);
            }
        }
    }
    return tsorted_pages;
}

}

int main() {
    test_split();

    std::ifstream inf("input.txt");
    PageOrderIndex order_index{readOrderingRules(inf)};

    // checking updates
    size_t res = 0;
    std::string line;
    while (std::getline(inf, line)) {
        // std::cout << line;

        std::unordered_set<std::string_view> forbidden;
        bool valid = true;

        auto pages = split(line, ',');
        for (auto page : pages) {
            if (forbidden.find(page) != forbidden.end()) {
                valid = false;
                break;
            }
            if (auto* gtPages = order_index.ltPageViews(page)) {
                for (auto page : *gtPages) {
                    forbidden.emplace(page);
                }
            }
        }
        if (!valid) {
            auto mid = pages.size() / 2;
            assert(mid < pages.size());
            assert(pages.size() % 2 == 1);
            
            auto tsorted_pages = topoligical_sort(pages, order_index);
            if (tsorted_pages.size() != pages.size()) {
                std::cerr << "Tsorted: " << tsorted_pages << std::endl;
                std::cerr << "Pages: " << pages << std::endl;
                continue; 
            }
            // assert(tsorted_pages.size() == pages.size()); // to catch the case with non-unique pages
            res += atoi(std::string(tsorted_pages[mid]).c_str());
        }
    }

    std::cout << res << std::endl;

    return 0;
}