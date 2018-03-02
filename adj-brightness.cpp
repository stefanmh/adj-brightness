#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstdlib>
#include <experimental/filesystem>
#include <sys/types.h>
#include <unistd.h>

namespace fs = std::experimental::filesystem::v1;

int read_int(std::string const &file)
{
    int val;
    std::ifstream f(file);
    if (!f || !(f >> val)) {
        std::cerr << "Cannot retrieve value from " << file << "." << std::endl;
        std::exit(-1);
    }
    return val;
}

void set_int(std::string const &file, int val)
{
    std::ofstream f(file);
    if (!f || !(f << val)) {
        std::cerr << "Cannot write value " << val << " to " << file << "." << std::endl;
        std::exit(-1);
    }
    std::cout << "Updated " << file << " to " << val << "." << std::endl;
}

void update_brightness(fs::path const &dir, int perc)
{
    int max_br = read_int(dir / "max_brightness");
    int crt_br = read_int(dir / "brightness");
    int new_br = std::max(std::min(max_br, int(crt_br + perc / 100.0 * max_br)), 1);
    set_int(dir / "brightness", new_br);
}

int main(int argc, char *argv[])
{
    try {
        int perc = 0;
        if (argc > 1) perc = std::atoi(argv[1]);
        else perc = 10;

        if (perc > 100 || perc < -100 || !perc) {
            std::cerr << "Syntax: " << argv[0] << "<+|-><perc>\n" << std::endl;
            return 1;
        }

        if (seteuid(0) < 0) {
            std::cerr << "Cannot get root" << std::endl;
            return 1;
        }

        for (auto &p: fs::directory_iterator("/sys/class/backlight")) {
            //if (!p.is_directory()) continue;
            update_brightness(p.path(), perc);
        }
    } catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
