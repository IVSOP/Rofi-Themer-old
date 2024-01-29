#include "data.h"

Data::Data(const std::string &dir)
: color_icons(), main_table()
{
	parseColors(dir + "color-icons");
	parseTable(dir + "main.tb");
}

void Data::parseColors(const std::string &path) {
	
}

void Data::parseTable(const std::string &path) {

}
