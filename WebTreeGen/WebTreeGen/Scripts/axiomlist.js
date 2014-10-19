function addAxiom() {
    var list = document.querySelectorAll("#axiom-list > div");
    var length = list.length;

    for (i = 0; i < length; i++) {
        var id = "#sys_axiom_" + i;
        if ($(id).is(":hidden")) {
            $(id).show();
            break;
        }
    }

    //          var txt = document.createTextNode(document.querySelectorAll("#axiom-list > div").length + 1 + ": ");
    //           r.appendChild(txt);
    //           var y = document.createElement("INPUT");
    //           y.setAttribute("type", "text");
    //           y.setAttribute("name","sys.axiom["+id+"]");
    //           y.setAttribute("value", "f");
    //           y.setAttribute("class", "text-box single-line");
    //           r.appendChild(y);
}