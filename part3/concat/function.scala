def concat(strings: String*) = {
    var ret = ""
    for (word <- 0 until strings.size) {
        ret += strings(word)
    }

    ret
}