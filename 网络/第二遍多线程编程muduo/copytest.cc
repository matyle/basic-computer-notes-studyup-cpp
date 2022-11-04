#include <iostream>
#include <string>
#include <memory>
using namespace std;
 
class Student
{
private:
	int num;
	//char *name;
    //string name;
    shared_ptr<char> name;
public:
	Student();
    //Student(const Student& that);
	~Student();
};
 
Student::Student()
{
	//name = new char(20);
    name = make_shared<char> (20);
	cout << "Student" << endl;
 
}

//默认只是指了指针 浅拷贝如下
// Student::Student(const Student& that){
//     name = that.name;
//     cout<<"copy constructor"<<endl;
// }


//默认只是指了指针
// Student::Student(const Student& s){
//     name = new char(20);
//     memcpy(name,s.name,strlen(s.name));
//     cout<<"copy constructor"<<endl;
// }
Student::
Student::~Student()
{
	cout << "~Student: " << name << endl;
	// delete name;
	// name = NULL;
}

 
int main()
{
	{// 花括号让s1和s2变成局部对象，方便测试
		Student s1;
		Student s2(s1);// 复制对象
		//Student s2 = s1;// 复制对象

	}
	system("pause");
	return 0;
}