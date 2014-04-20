package operations;

import exceptions.DivisionByZeroArithmeticException;
import exceptions.OverflowArithmeticException;
import types.INumber;

/**
 * Created by pva701 on 3/15/14.
 */

public interface Expression3<T> {
    public T evaluate(T x, T y, T z) throws ArithmeticException;
}

//чем отличаются интерфейсы от абстрактных классов со ВСЕМИ абстрактными методами без полей и констант?
//1) нельзя делать множественное наследование классов, но можно реализовывать несколько интерфейсов
//2) нельзя создавать абстрактные классы// можно??????

//почему абстрактные методы нельзя реализовывать в классе-родителе, наподобие virtual int f(); в с++? как это сделать в Java?
//1) экземпляры абстрактного класса нельзя создавать, но если мы хотим вынести общий кусок в родительский класс, и вызывать super.f(),
//то как быть?

//правильно я понимаю, что нет смысла создавать final переменные в абстр. классе, т.к. у него нет конструктора, но ведь классы-потомки
//могут установить значения этим переменным

//почему не рантаймит Integer при переполнении?
