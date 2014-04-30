vars = [];
vars.push("x");
vars.push("y");
vars.push("z");

function containsVars(s) {
    for (var i = 0; i < vars.length; ++i)
        if (vars[i] === s) return i;
    return undefined;
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

var add = binaryOperation(opAdd);
var subtract = binaryOperation(opSub);
var multiply = binaryOperation(opMul);
var divide = binaryOperation(opDiv);
var abs = function(f) {
            return function(x, y, z) {
                return Math.abs(f(x, y, z));
            }
}

var log = function(f) {
    return function(x, y, z) {
        return Math.log(f(x, y, z));
    }
}

funs = [];
funs["log"] = {f:Math.log, arn:1};
funs["abs"] = {f:Math.abs, arn:1};
funs["pow"] = {f:Math.pow, arn:2};
funs["+"]   = {f:opAdd,    arn:2};
funs["-"]   = {f:opSub,    arn:2};
funs["*"]   = {f:opMul,    arn:2};
funs["/"]   = {f:opDiv,    arn:2};


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

            var addf = function(cfun, par) {
                   return function() {
                       var localPar = par;
                       var localCfun = cfun;
                       var numb = [];
                       for (var i = 0; i < localPar.length; ++i)
                           numb[i] = localPar[i].apply(null, arguments);
                       return localCfun.apply(null, numb.reverse());
                   }
                }
            stk.push(addf(fun, parametrs))
        } else if (containsVars(token) != undefined)
            stk.push(variable(token));
         else
            stk.push(cnst(Number(token)));
    }
    return stk.pop();
}

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
