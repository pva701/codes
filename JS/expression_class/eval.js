var Const = function(val) {
        this.evaluate = function(x) {
            return val;
        }
        this.toString = function() {
            return String(val); 
        }

        this.diff = function(name) {
            return new Const(0);
        }
}

var Variable = function(name) {
    this.evaluate = function(x) {
        return x; 
    }
    this.toString = function() {
        return name;
    }
    this.diff = function(name) {
        if (name === "x")
            return new Const(1);
        return new Const(0);
    }
}

//Interface for binary operations
var BinaryOperation = {};
BinaryOperation.evaluate = function(x) {
    return this.operation(this.f.evaluate(x), this.g.evaluate(x));
}
BinaryOperation.toString = function() {
    return this.f.toString() + " " + this.g.toString() + " " + this.sign;
}
BinaryOperation.diff = function(name) {
    return this.diffRule(this.f, this.g, name);
}
BinaryOperation.getSign = function() {
    return this.sign;
}

BinaryOperation.initializate = function(f, g, Map) {
    this.f = f;
    this.g = g;
    this.operation = Map.operation;
    this.sign = Map.sign;
    this.diffRule = Map.diffRule;
}

//Multiply
var Multiply = function(f, g) {
    this.initializate(f, g, Multiply);
}
Multiply.prototype = BinaryOperation;
Multiply.operation = function(x, y) {return x * y;};
Multiply.sign = "*";
Multiply.diffRule = function(u, v, varbl) {return new Add(new Multiply(u.diff(varbl), v), new Multiply(u, v.diff(varbl)));};

//Add
var Add = function(f, g) {
    this.initializate(f, g, Add);
}
Add.prototype = BinaryOperation;
Add.operation = function(x, y) {return x + y;};
Add.sign = "+";
Add.diffRule = function(u, v, varbl) {return new Add(u.diff(varbl), v.diff(varbl));};

//Divide
var Divide = function(f, g) {
    this.initializate(f, g, Divide);
}
Divide.prototype = BinaryOperation;
Divide.operation = function(x, y) {return x / y;};
Divide.sign = "/";
Divide.diffRule = function(u, v, varbl) {
    return new Divide(new Subtract(new Multiply(u.diff(varbl), v), new Multiply(u, v.diff(varbl))), new Multiply(v, v));};

//Subtract
var Subtract = function(f, g) {
    this.initializate(f, g, Subtract);
}
Subtract.prototype = BinaryOperation;
Subtract.operation = function(x, y) {return x - y;};
Subtract.sign = "-";
Subtract.diffRule = function(u, v, varbl) {return new Subtract(u.diff(varbl), v.diff(varbl));}

var parse = function(s) {
    var stk = [];
    var tokens = s.split(" ");
    var binOpMap = [];
    binOpMap["*"] = Multiply;
    binOpMap["+"] = Add;
    binOpMap["/"] = Divide;
    binOpMap["-"] = Subtract;
    var vars = [];
    vars["x"] = true;

    for (var i = 0; i < tokens.length; ++i) {
        var token = tokens[i];
        if (token === "")
            continue;
        var found = binOpMap[token];
        if (found != null) {
            var arg2 = stk.pop();
            var arg1 = stk.pop();
            stk.push(new found(arg1, arg2));
            continue;
        }
        
        if (vars[token] != null) {
            stk.push(new Variable(token));
            continue;
        }

        stk.push(new Const(Number(token)));
    }
    return stk.pop();
}
/*var exp = new BinaryOperation();
for (var key in expr)
    if (exp.hasOwnProperty(key))
        println(key);*/
//println(expr.evaluate(5));
//println(new Multiply(new Const(2), new Const(3)).toString());
println(parse("x x 2 - * x * 1 +").diff("x").evaluate(5));

var expr = new Subtract(
            new Multiply(
                new Const(2), 
                new Variable("x")),
            new Const(3));
println(expr.prototype);
println(expr.diff("x").toString());
println(new Subtract(new Multiply(new Const(2), new Const(4)), new Multiply(new Add(new Const(3), new Variable("x")), new Variable("x"))).diff("x").toString());
for (var i = 0; i <= 10; ++i)
    println(i, " ", expr.evaluate(i));
