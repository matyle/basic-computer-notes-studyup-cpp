```go

package main

func mergeDualExchangeCfgFields(to, from *stu Student) error {
	tov := reflect.ValueOf(to)
	fromv := reflect.ValueOf(from)
	return mergeDualExchWholePtrValue(tov, fromv)
}

func mergeDualExchWholePtrValue(to, from reflect.Value) error {
	if to.Type().Kind() != from.Type().Kind() {
		panic("bug")
	}

	to = to.Elem()
	from = from.Elem()

	for index := 0; index < from.NumField(); index++ {
		//structFieldType := from.Type().Field(index)
		fromField := from.Field(index)

		// 仅合并指针字段
		if fromField.Kind() != reflect.Ptr {
			continue
		}

		// 忽略空值
		if fromField.IsNil() {
			continue
		}

		toField := to.Field(index)
		//子结构需要判断，其中是否有空
		if fromField.Elem().Kind() == reflect.Struct {
			//递归处理子结构
			if err := mergeDualExchWholePtrValue(toField, fromField); err != nil {
				return err
			}
			continue

		}
		// 其余，则直接指针转移
		toField.Set(fromField)
	}
	return nil
}


```