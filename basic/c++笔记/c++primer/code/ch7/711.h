#include<iostream>
#include<string>
class Sales_data{
public:
    Sales_data(std::istream& is){
        double price = 0;
        is >> bookNo >> units_sold >> price;
        revenue = units_sold * price;
    }
    std::string bookNo;
    unsigned int units_sold;
    double revenue;
    std::string isbn() const{
        return bookNo;
    }
    Sales_data &combine(const Sales_data& item); //+=

};
Sales_data add(const Sales_data& lhs, const Sales_data& rhs); //将s1加到s2
std::istream& read(std::istream&,Sales_data& s);
std::ostream &print(std::ostream&,const Sales_data& s);
