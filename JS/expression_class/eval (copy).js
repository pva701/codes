var Const = function(val) {
        this.evaluate = function(x) {
            return val;
        }
        this.toString = function() {
            return val; 
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

var BinaryOperation = function(f, g, operation, sign, diffRule) {
    this.initializate = function(curBin) {
        this.evaluate = curBin.evaluate;
        this.toString = curBin.toString;
        this.diff     = curBin.diff;
        this.getSign  = curBin.getSign;
    }

    this.evaluate = function(x) {
        return operation(f.evaluate(x), g.evaluate(x));
    }
    this.toString = function() {
        return f.toString() + " " + g.toString() + " " + sign;
    }
    this.diff = function(name) {
        return diffRule(f, g, name);
    }
    this.getSign = function() {
        return sign;
    }
}

var Multiply = function(f, g) {
    var diffRule = function(u, v, varbl) {
        return new Add(new Multiply(u.diff(varbl), v), new Multiply(u, v.diff(varbl)));
    }
    var bin = new BinaryOperation(f, g, function(x, y) {return x * y;}, "*", diffRule);
    bin.initializate.call(this, bin);
}


var Add = function(f, g) {
    var diffRule = function(u, v, varbl) {
        return new Add(u.diff(varbl), v.diff(varbl));
    }
    var bin = new BinaryOperation(f, g, function(x, y) {return x + y;}, "+", diffRule);
    bin.initializate.call(this, bin);
}


var Divide = function(f, g) {
    var diffRule = function(u, v, varbl) {
        return new Divide(new Subtract(new Multiply(u.diff(varbl), v), new Multiply(u, v.diff(varbl))), new Multiply(v, v));
    }
    var bin = new BinaryOperation(f, g, function(x, y) {return x / y;}, "/", diffRule);
    bin.initializate.call(this, bin);
}

var Subtract = function(f, g) {
    var diffRule = function(u, v, varbl) {
        return new Subtract(u.diff(varbl), v.diff(varbl));
    }
    var bin = new BinaryOperation(f, g, function(x, y) {return x - y;}, "-", diffRule);
    bin.initializate.call(this, bin);
}

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

println(new Add(new Const(2), new Const(2)));

println(parse("x x 2 - * x * 1 +").diff("x").evaluate(5));

var expr = new Subtract(
            new Multiply(
                new Const(2), 
                new Variable("x")),
            new Const(3));
println(expr.diff("x").toString());
println(new Subtract(new Multiply(new Const(2), new Const(4)), new Multiply(new Add(new Const(3), new Variable("x")), new Variable("x"))).diff("x").toString());
for (var i = 0; i <= 10; ++i)
    println(i, " ", expr.evaluate(i));
