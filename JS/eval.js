"use strict";
var vars = [];
vars.push("x");
vars.push("y");
vars.push("z");

function containsVars(s) {
    for (var i = 0; i < vars.length; ++i)
        if (vars[i] === s) return i;
}

function cnst(value) {
    return function() {//take some arguments, arguments.length == var.length
        return value;
    }
}

function variable(name) {
    return function() {//take some arguments, arguments.length == var.length
        return arguments[containsVars(name)];
    }
}

function binaryOperation(op) {
    return function(f, g) {
            return function(x, y, z) {
                return op(f(x, y, z), g(x, y, z));
            }
    }
}

function opAdd(x, y) {return x + y;}
function opSub(x, y) {return x - y;}
function opMul(x, y) {return x * y;}
function opDiv(x, y) {return x / y;}

/*var add = binaryOperation(opAdd);
var subtract = binaryOperation(opSub);
var multiply = binaryOperation(opMul);
var divide = binaryOperation(opDiv);*/

function makeFunction(f) {
    return function() {
        var functions = arguments;
        return function() {    
            var results = [];
            for (var i = 0; i < functions.length; ++i)
                results[i] = functions[i].apply(null, arguments);
            return f.apply(null, results);
        }

    }
}
var abs = makeFunction(Math.abs);
var log = makeFunction(Math.log);
var add = makeFunction(opAdd);
var subtract = makeFunction(opSub);
var multiply = makeFunction(opMul);
var divide = makeFunction(opDiv);

var funs = [];
funs["log"] = {f:makeFunction(Math.log), arn:1};
funs["abs"] = {f:makeFunction(Math.abs), arn:1};
funs["pow"] = {f:makeFunction(Math.pow), arn:2};
funs["+"]   = {f:makeFunction(opAdd),    arn:2};
funs["-"]   = {f:makeFunction(opSub),    arn:2};
funs["*"]   = {f:makeFunction(opMul),    arn:2};
funs["/"]   = {f:makeFunction(opDiv),    arn:2};

function parse(s) {
    var stk = [];
    var tokens = s.split(" ");
    for (var i = 0; i < tokens.length; i++) {
        var token = tokens[i];
        if (token === "") 
            continue;
        if (token in funs) {
            var fun = funs[token].f; 
            var arnost = funs[token].arn;
            var parametrs = [];
            for (var j = 1; j <= arnost; j++) 
                parametrs.push(stk.pop());
            stk.push(fun.apply(null, parametrs.reverse()));
        } else if (containsVars(token) != undefined)
            stk.push(variable(token));
         else
            stk.push(cnst(Number(token)));
    }
    return stk.pop();
}
 
//idaeje
/*println(parse("x x +")(1, 2, 3));
println(parse("x x + y * 5 +")(1, -2, 3));
println(parse("x abs")(-1, 2, 3));
println(parse("x y +")(1, 2, 3));
println(parse("585820722 1765421938 - 1263385401 1930043222 / y * -")(0, 0, 1));
var expr = parse("585820722 1765421938 - 1263385401 1930043222 / y * -");
//var expr = parse("585820722 1765421938 -");
println(expr(0, 0, 0))
println(expr(0, 0, 1));*/
//println(parse("a x + a *")(1, 2, 3, 4));
println(parse("2093101096 1943850576 /")(0,0,0));
println(parse("2093101096 1943850576 /")(0,0,0));
var expr = parse("2093101096 1943850576 /");
println(expr(1, 2, 3));
println(expr(3, 4, 5));
println(parse("x y pow")(3, 4, 10));
//println(abs(cnst(-3))(1, 2, 3));
println(multiply(variable("y"), variable("y"))(1, 2, 3));
//println(subtract(subtract(multiply(variable("y"),variable("y")),divide(variable("y"),log(abs(variable("x"))))),cnst(1014523727))(0,0,0));
