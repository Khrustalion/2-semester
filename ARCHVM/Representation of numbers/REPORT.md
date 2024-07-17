| Лабораторная работа №1 | M3101       | АОВС |
| ---------------------- | ----------- | ---- |
| Представление чисел    | Хрусталев Николай Дмитриевич       | 2024 |


## Вариант
Была выполнена упрощенная часть работы обеих работ: округление только к 0

## Результат работы на тестовых данных:
[последний успешный action](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-fixed-floating-Khrustalion/actions/runs/8200280668)

# Описание:

## Фиксированная точка

- Был создан класс FixedPoint с перегруженными операторами +, -, *, /, а также оператором вывода для вычислиения соответствующих операций. Дополнтельно была создана функция rounding(), которая вызывается для округления при выводе.

- числа хранятся в формате long long(int64_t), в этом числе подряд(слева направо) идут сначала целая часть, затем дробная. Также отедально хранятся int_part и fractional_part(в типе long long) для удобства вычислений. Для удобства вывода хранится поле digit3, которое сохраняет 3 цифры после запятой для вывода. Все хрнаится в формате, так как по условию лабораторной А + В <= 32, а так при операции умножения максимум можно получить 64 бита, и чтобы не было потерь в старших битах и все числа всегда влезали, то был использован формат long long.

- Округление при выводе осуществляется при помощи функции rounding(). 
> Во всех случаях округления, если число отрицательно и ```digit3 != 0```, то итоговую ```digit3``` вычисляем, как ```digit3 = (1000 - digir3) % 1000```(так как при отрицательной целой части мы как будто вычитаем этудробную часть из 1).
1. Округление к 0 зависит от знака. Если число отрицательное и ```fractional_part != 0```, то мы должны увеличить ```int_part``` на 1, а ```fractional_part``` должны вычесть из "1" (1 в том формате, который ввел пользователь, например, если пользователь ввел формат 10.10, то 1 в таком формате будет равна ``` 1 << 10```). После этого приравниваем ```digit3 = (fractional_part*1000) >> B```. Таким образом мы получаем 3 цифры после запятой, в дальнейшем таким образом будут получаться 3 цифры для всех видов округления.

- Сложение и вычитание сделано с помощью целочисленных вычисления. Возвращаем новый экземпляр класса c числом, из которого мы получаем целую и дробную часть, равным сумме двух входных

```return FixedPoint(A_, B_, rounding_, number_ + rhs.number_);```

- Вычитаение сделано аналигочно сложению, за исключением на проверки случая, когда ```rhs.number_ > number_```, в этом случае возвращаем экземпляр класса с чсилом, равным ```-(rhs.number_ - number_)```

```
if (number_ < rhs.number_) {
    return FixedPoint(A_, B_, rounding_, -(rhs.number_ - number_));
}
return FixedPoint(A_, B_, rounding_, number_ - rhs.number_);
```

- Результат умножения получаем путем умножения двух чисел, предстваляющих два входных числа. После этот результат необходимо сдиванут на B бит, чтобы резльутат был в корректном формате. ```long long answer = (number_ * rhs.number_) >> B_;```. Также создаем переменную ```least_significant_bits```, в которую сохраняем биты, которые потеряются при операции битового сдвига, и на основе этих битов делаем округление операции.

- В операции деления сначала делаем проверку на ноль второго числа. После этого необходимо привести оба числа в доп код, так как это важно при делении. Делаем это путем битовых операций. После этого возвращаем экземпляр класса с числом, равным частному первого числа и второго. Также необходимо сдвинуть делимое на B битов, чтобы результать был в корректном формате. ```return FixedPoint(A_, B_, rounding_, (lhs << B_) / rhs_);```.

## Плавающая точка
- Был создан класс FloatPoint с перегруженными операторами +, -, *, /, а также оператором вывода для вычислиения соответствующих операций. Дополнтельно была создана функция rounding(), которая вызывается при выполнении некоторых операций, в которых происходит потеря младших битов. Также были добавлены вспоготальные методы (```isZero()```, ```isNan()```, ```isInf()```), при вызове котрых возвращается ```true```, если соответвсно число нулевое, NaN, или бесконечность.

- Все необходимые части числа хрантся в приватном поле класса и могут быть вызваны соответсвующими методамии. Мантисса хранится в формате ```long long(int64_t)```, экспонента с типом округления хранятся в формате ```int```. Также есть два флага ```is_null_``` (сообщает о нулевой экспоненте: ```true```, если экспонента нулевая), ```neg_sign_``` (сообщает о знаке числа: ```true```, если число отрицательно)

1. Округление к 0 сделано с помощью потери битов. В функцию ```toward_zero(int bit, long long mantissa)``` передается то количество битов, которое необходимо округлить и само число. Далее идет сдвиг битов на то количество, которое было передано (направление сдвига определяется знаком перемнной ```bit```)

> Во всех операций сначала идут проверки на критические случаи: ```Nan```, ```+inf + (-inf)``` и т. п.. После проверок на критические случаи идут проверки на знак. И в итоге запускается операция только от двух положительных чисел. Проверка на принадлежность числа на "критическое", производится путем соответствующего метода ```is<>```, где <> - это ```Nan```, ```inf``` или ```Zero```. Эти метода проверяют сначала значение экспоненты: если она равна максимальному значение, то это бесконечность или nan(далее смотрят на мантиссу), если она равна минимальному значению, то это денормализованное число. Далее смотрят на мантиссу: если в первом случае мантисса нулевая, то это бесконечность, инче nan. Во втором случае число будет нулевым, если мантисса нулевая, иначе, это обычное денормализованное число.

- Сложение сделано с помощью целочисленных вычислений. После всех проверок получаем переменную ```int new_exponent = std::max(exponent_, rhs.exponent_);``` - экспонента нового числа (максимум, так как привожу меньшую экспоненту к большей). Далее в переменные ```lhs_number```, ```rhs_number``` добваляю "мнимые единицы", если это нормализированные числа. Далее получаю переменную ```new_mantissa += lhs_mantissa + rhs_mantissa;```, которая является результатом сложения (из этого числа легко получить искомую мантиссу). Далее идут проверки на нормализированность полученного результат и корректировки экспоненты.

- Вычитание сделано аналогично сложению, за исключением некоторых вещей. Запускается операция вычитания, только когда левое число больше правого. И в случае, если разность экспонент будет больше размера мантиссы, то необходимо вычесть 1 из левого числа (так же, если бы мы делали это руками), так как иначе правое число при округлении к 0 полностью обнулится и ответ будет неверным.

- Умножение сделано с помощью целочисленных вычислений. После всех проверок полчаем переменную ```long long new_exponent = exponent_ + rhs.exponent_;``` - экспонента нового числа (при перемножении степеи скалдываются). Далее в переменные ```lhs_number```, ```rhs_number``` добваляю "мнимые единицы", если это нормализированные числа. Далее получаю переменную ```long long result = rounding(size_man, rhs_number * lhs_number);```, которая является результатом произведения (из этого числа легко получить искомую мантиссу). Далее идут проверки на нормализированность полученного результат и корректировки экспоненты. Например, если на каком-то этапе экспонента больше или равна максимальному значению экспоненты, то по стандарту это число следует округлить к максимально возможному, делаем это таким образом: ```FloatPoint(((static_cast<int64_t>(1) << (size_man + size_exp)) - 1) ^ (1 << size_man), rounding_);```. Если экспоненты меньше, чем минимально возможная, то пытаемся скорректировать ее к минимально возможной. Если после этого мантисса станет равной 0, а эксмонента будет меньше либо равна минимально возможной, то такой случай по стандарту следует округлить к 0. Получилось скорректировать экспоненту до минимально возможной и при этой мантисса не равно 0, то мы имеем дело с число, нормализованность которого зависит от размера числа: если оно занимает больше бит, чем размер мантисса, то это нормализированное число, иначе это денормализированное.

- Деление сделано аналогично умножнию. После всех проверок полчаем переменную ```long long new_exponent = exponent_ - rhs.exponent_;``` - экспонента нового числа (при пделении степеи вычитаются). Дальнейшие случаи разобраны в абзаце с умножением.