def concat_ws(sep: String, strings: String*) = {
    var ret = ""
    for (word <- 0 until (strings.size)-1) {
        ret += strings(word)
        ret += sep
    }

    ret += strings((strings.size)-1)
    ret
}