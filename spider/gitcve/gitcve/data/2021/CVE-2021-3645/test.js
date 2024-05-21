var merge = require("..")
var a = {"a":{"red":"apple"}}
var b = {"b":{"yellow":"mango"}}
var c = {"a":{"orange":"orange"}}
merge(a,b,c)
console.log(a)