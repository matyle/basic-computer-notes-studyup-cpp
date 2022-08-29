#include "711.h"

Sales_data &Sales_data::combine(const Sales_data& rhs){
    if(this->bookNo==rhs.bookNo){
        this->units_sold += rhs.units_sold;
        this->revenue += rhs.revenue;
    }
    return *this;
}
//item从is中读入
std::istream &read(std::istream &is,Sales_data &item){
    double price = 0;
    is >> item.bookNo >> item.units_sold >> price;
    item.revenue = item.units_sold * price;
    return is;

}

std::ostream &print(std::ostream &os,const Sales_data &item){
    os << item.isbn() << " " << item.units_sold << " " <<item.revenue << " ";
    return os;

}

Sales_data add(const Sales_data& lhs, const Sales_data& rhs){
    Sales_data sum = lhs; //左值赋给sum
    sum.combine(rhs);
    return sum;
}

