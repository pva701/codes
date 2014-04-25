function cnst(value) {
    return function(x) {
        return value;
    }
}


function variable(name) {
    return function(x) {
        return x;
    }
}

function binaryOperation(op) {
    return function(f,g) {
            return function(x) {
                return op(f(x), g(x));
            }
    }
}

function opAdd(x, y) {return x + y;}
function opSub(x, y) {return x - y;}
function opMul(x, y) {return x * y;}
function opDiv(x, y) {return x / y;}

var add = binaryOperation(opAdd);
var subtract = binaryOperation(opSub);
var multiply = binaryOperation(opMul);
var divide = binaryOperation(opDiv);

for (var i = 0; i <= 10; ++i) {
    var expr = add(subtract(multiply(variable("x"), variable("x")), multiply(cnst(2), variable("x"))), cnst(1));
    println(expr(i));
}

function parse(s) {
    stk = [];
    for (var i = 0; i < s.length; i++) {
        switch (s[i]) {
            case "x":
                stk.push(variable("x"));
                break;
            case "+":
                stk.push(add(stk.pop(), stk.pop()));
                break;
            case "-":
                var arg = stk.pop();
                stk.push(subtract(stk.pop(), arg));
                break;
            case "*":
                stk.push(multiply(stk.pop(), stk.pop()));
                break;
            case "/":
                var arg = stk.pop();
                stk.push(divide(stk.pop(), arg));
                break;
            default:
                stk.push(cnst(Number(s[i])));
        }
    }
    return stk.pop();
}
println(parse("xx2-*x*1+")(5));

