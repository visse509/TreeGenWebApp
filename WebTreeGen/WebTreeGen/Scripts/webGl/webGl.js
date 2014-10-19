var modelData = "<root>";
var modelDataXml
$(document).ready(function () {

  //  setTimeout("getUpdate()", 3000);
});
function getUpdate(filename) {
    if (filename == null) {
        filename = "default"
    }
    $.ajax({
        url: "/Properties/getData/",
        type: "POST",
        data: { input: filename},
        timeout: 10000,
        dataType: "text",
        success: function (result) {
            modelData = result
            modelDataXml = $(modelData);
            parseModelXml()
        }
    });

}

function parseModelXml() {
    modelDataXml
    var dataList = [];
    var indexList = [];

    var branchList = modelDataXml.find("data");

    for (k = 0; k < branchList.length; k++) {
        var vertList = branchList[k].innerHTML; ;
        var vert = vertList.split("\n");
        for (j = 1; j < vert.length - 1; j++) {

            var floatData = vert[j].split("\t");
            for (i = 0; i < floatData.length; i++) {
                var str = floatData[i].replace(/\[/g, "");
                dataList.push(parseFloat(str));
            }
        }
    }
    var branchList = modelDataXml.find("indices");
    for (k = 0; k < branchList.length; k++){
        var indices = branchList[k].innerHTML;
        var index = indices.split("\n");
        var largest = 0;
        if (indexList.length > 0) {
            var largest = Math.max.apply(Math, indexList) + 1;
            indexList.pop();
        }
        for (i = 1; i < index.length; i++) {
            var str = index[i].replace(/\,/g, "");
             var tempInt = parseInt(str) >>> 0;
             indexList.push(tempInt + largest);
        }
     }
     clearBuffer();
    initBuffers(dataList,indexList);
}

function createGL() {

    var canvas = document.getElementById("glCanvas");
    var filename = $('#title').val();
    getUpdate(filename);
    initGL(canvas);
    initShaders();
    initBuffers([],[]);

    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.enable(gl.DEPTH_TEST);

    canvas.onmousedown = handleMouseDown;
    document.onmouseup = handleMouseUp;
    document.onmousemove = handleMouseMove;

    tickScene();
}

function tickScene() {
    requestAnimFrame(tickScene);
    drawScene();
}


var gl;
function initGL(canvas) {
    try {
        gl = canvas.getContext("experimental-webgl");
        gl.viewportWidth = canvas.width;
        gl.viewportHeight = canvas.height;
    } catch (e) {
    }
    if (!gl) {
        alert("Could not initialise WebGL");
    }
}
var mvMatrix = mat4.create();
var pMatrix = mat4.create();

var shaderProgram;
function initShaders() {
    var fragmentShader = getShader(gl, "shader-fs");
    var vertexShader = getShader(gl, "shader-vs");

    shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);

    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        alert("Could not initialise shaders");
    }

    gl.useProgram(shaderProgram);

    shaderProgram.vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
    gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);

    shaderProgram.pMatrixUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
    shaderProgram.mvMatrixUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
}

function getShader(gl, id) {
    var shaderScript = document.getElementById(id);
    if (!shaderScript) {
        return null;
    }

    var str = "";
    var k = shaderScript.firstChild;
    while (k) {
        if (k.nodeType == 3)
            str += k.textContent;
        k = k.nextSibling;
    }

    var shader;
    if (shaderScript.type == "x-shader/x-fragment") {
        shader = gl.createShader(gl.FRAGMENT_SHADER);
    } else if (shaderScript.type == "x-shader/x-vertex") {
        shader = gl.createShader(gl.VERTEX_SHADER);
    } else {
        return null;
    }

    gl.shaderSource(shader, str);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert(gl.getShaderInfoLog(shader));
        return null;
    }

    return shader;
}

var mvMatrix = mat4.create();
var mvMatrixStack = [];
var pMatrix = mat4.create();

function mvPushMatrix() {
    var copy = mat4.create();
    mat4.set(mvMatrix, copy);
    mvMatrixStack.push(copy);
}

function mvPopMatrix() {
    if (mvMatrixStack.length == 0) {
        throw "Invalid popMatrix!";
    }
    mvMatrix = mvMatrixStack.pop();
}

function setMatrixUniforms() {
    gl.uniformMatrix4fv(shaderProgram.pMatrixUniform, false, pMatrix);
    gl.uniformMatrix4fv(shaderProgram.mvMatrixUniform, false, mvMatrix);
}

var squareVertexPositionBuffer;
var cubeVertexIndexBuffer;

function initBuffers(vertexList, indexList) {
    
    squareVertexPositionBuffer = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
    vertices = vertexList

    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
    squareVertexPositionBuffer.itemSize = 3;
    squareVertexPositionBuffer.numItems = vertices.length;

    cubeVertexIndexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, cubeVertexIndexBuffer);
    var cubeVertexIndices = indexList
    //var cubeVertexIndices = [0,9,1,1,9,10,1,10,2,2,10,11,2,11,3,3,11,12,3,12,4,4,12,13,4,13,5,5,13,14,5,14,6,6,14,15,6,15,7,7,15,16,7,16,8,8,16,17,9,18,10,10,18,19,10,19,11,11,19,20,11,20,12,12,20,21,12,21,13,13,21,22,13,22,14,14,22,23,14,23,15,15,23,24,15,24,16,16,24,25,16,25,17,17,25,26]
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(cubeVertexIndices), gl.STATIC_DRAW);
    cubeVertexIndexBuffer.itemSize = 1;
    cubeVertexIndexBuffer.numItems = cubeVertexIndices.length;

   
}
function clearBuffer() {
    gl.deleteBuffer(squareVertexPositionBuffer);
    gl.deleteBuffer(cubeVertexIndexBuffer);
    
}
function drawScene() {

    
    gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    mat4.perspective(45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix);

    mat4.identity(mvMatrix);
    mvPushMatrix();
    mat4.translate(mvMatrix, [-1.5, -10, -25]);
    mat4.multiply(mvMatrix, cameraTranslationMatrix);
    
    
    
    gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
    gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, squareVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);

    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, cubeVertexIndexBuffer);
    setMatrixUniforms();
    gl.drawElements(gl.TRIANGLES, cubeVertexIndexBuffer.numItems, gl.UNSIGNED_SHORT, 0);

    mvPopMatrix();
}

var mouseDown = false;
var lastMouseX = null;
var lastMouseY = null;
var cameraTranslationMatrix = mat4.create();
mat4.identity(cameraTranslationMatrix);

function handleMouseDown(event) {
    mouseDown = true;
    lastMouseX = event.clientX;
    lastMouseY = event.clientY;
}
function handleMouseUp(event) {
    mouseDown = false;
}

function handleMouseMove(event) {
    if (!mouseDown) {
        return;
    }
    var newX = event.clientX;

    var deltaX = newX - lastMouseX;
    var newRotationMatrix = mat4.create();
    mat4.identity(newRotationMatrix);

    mat4.rotate(newRotationMatrix, degToRad(deltaX / 5), [0, 1, 0]);

    mat4.multiply(newRotationMatrix, cameraTranslationMatrix, cameraTranslationMatrix);

    lastMouseX = newX
}

function degToRad(deg) {
    return (deg*Math.PI)/180;
}