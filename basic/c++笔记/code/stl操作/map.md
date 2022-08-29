map中的lower_bound和upper_bound的意思其实很简单，就两句话：

map::lower_bound(key):返回map中第一个大于或等于key的迭代器指针

map::upper_bound(key):返回map中第一个大于key的迭代器指针

所以，理解这两个函数请不要按照字面意义思考太复杂，因为仅仅是不小于（lower_bound）和大于（upper_bound）这么简单。