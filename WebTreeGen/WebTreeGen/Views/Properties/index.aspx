<%@ Page Language="C#" Inherits="System.Web.Mvc.ViewPage<WebTreeGen.Models.TreeViewModel>" %>

<!DOCTYPE html>

<html>
<head id="Head1" runat="server">
    <title>Edit</title>
    <link href="~/Content/Site.css" rel="stylesheet" type="text/css" />
</head>

<body onload="createGL();">
    <script src="<%: Url.Content("~/Scripts/jquery-1.5.1.min.js") %>" type="text/javascript"></script>
    <script src="<%: Url.Content("~/Scripts/jquery.validate.min.js") %>" type="text/javascript"></script>
    <script src="<%: Url.Content("~/Scripts/jquery.validate.unobtrusive.min.js") %>" type="text/javascript"></script>
    <script src="<%: Url.Content("~/Scripts/webGl/webgl-utils.js") %>" type="text/javascript"></script>
    <script src="<%: Url.Content("~/Scripts/webGl/glMatrix-min.js") %>" type="text/javascript"></script>
    <script src="<%: Url.Content("~/Scripts/webGl/webGl.js") %>" type="text/javascript"></script>
    <script src="<%: Url.Content("~/Scripts/axiomlist.js") %>" type="text/javascript"></script>

        <script type="text/javascript">
            $("#girth").removeAttr("data-val");
            $("#segmentLength").removeAttr("data-val");


        </script>

    <input type="hidden" id = "title" value="<% Response.Write(Model.treeName); %>" name="title">
        <div id = "scene">
        <canvas id="glCanvas" style="border: none;" width="650" height="800">
   </div>
    
    <div>
     <% for (int i = 1; i < WebTreeGen.Models.Properties.propertiesList.Count; i++) %>
         <%{%>
         <%: Html.ActionLink("Level " + i, "Index", new { id = i, treeName = Model.treeName })%>
         <%}%>
    </div>


    <div id = "settings">
    <div id="settings-properties">
    <% using (Html.BeginForm()) { %>
        <%: Html.ValidationSummary(true) %>
        <fieldset>
            <legend>Properties</legend>
    
            <%: Html.HiddenFor(model => model.prop.ID)%>
    
            <div class="editor-label">
                <%: Html.LabelFor(model => model.prop.nSegments) %>
            </div>
            <div class="editor-field">
                <%: Html.EditorFor(model => model.prop.nSegments)%>
                <%: Html.ValidationMessageFor(model => model.prop.nSegments)%>
            </div>
    
            <div class="editor-label">
                <%: Html.LabelFor(model => model.prop.segmentLength)%>
            </div>
            <div class="editor-field">
                <%: Html.EditorFor(model => model.prop.segmentLength)%>
                <%: Html.ValidationMessageFor(model => model.prop.segmentLength)%>
            </div>
    
            <div class="editor-label">
                <%: Html.LabelFor(model => model.prop.nEdges)%>
            </div>
            <div class="editor-field">
                <%: Html.EditorFor(model => model.prop.nEdges)%>
                <%: Html.ValidationMessageFor(model => model.prop.nEdges)%>
            </div>
    
            <div class="editor-label">
                <%: Html.LabelFor(model => model.prop.girth)%>
            </div>
            <div class="editor-field">
                <%: Html.EditorFor(model => model.prop.girth)%>
                <%: Html.ValidationMessageFor(model => model.prop.girth)%>
            </div>
    
            <div class="editor-label">
                <%: Html.LabelFor(model => model.prop.nLeafs)%>
            </div>
            <div class="editor-field">
                <%: Html.EditorFor(model => model.prop.nLeafs)%>
                <%: Html.ValidationMessageFor(model => model.prop.nLeafs)%>
            </div>
    
        </fieldset>
        </div>
        <div id="Div1">
        <fieldset>
            <legend>L-System parameters</legend>
    
            <%: Html.HiddenFor(model => model.sys.ID)%>
    
            <div class="editor-label">
                Axiom
            </div>
             <% int l; %>
            <div class="editor-field" id="axiom-list">
            <%for (l = 0; l < Model.sys.axiom.Count; l++)
              { %>
                    <div>
                        <% Response.Write(l+1); %>:
                        <%: Html.EditorFor(model => model.sys.axiom[l])%>
                        <%: Html.ValidationMessageFor(model => model.sys.axiom[l])%>
                    </div>
               
                <% } %>

                <%for (int k = l; k < l+5; k++)
                { %>
                    <div style="display:none" id="sys_axiom_<% Response.Write(k); %>">
                        <% Response.Write(k+1); %>: <input class="text-box single-line" type="text" name="sys.axiom[<% Response.Write(k); %>]" >
                    </div>
               
                <% } %>
                
                 </div>
                 
                <button onclick="addAxiom()" type="button" style="margin-top:10px; margin-left:15px;">Add axiom</button><br/>
                <br />
                 <div class="editor-label">
                    Production rules
                </div>
                 <% var pRules = Model.sys.pRules.ToList();  %>
                <%for (int i = 0; i < pRules.Count; ++i)
                { %>
              

                <div class="editor-field">
          
                   <%: Html.EditorFor(model => pRules[i].Key)%> 
                    <%: Html.ValidationMessageFor(model => pRules[i].Key)%> 

                     <%: Html.EditorFor(model => pRules[i].Value)%> 
                     <%: Html.ValidationMessageFor(model => pRules[i].Value)%> 
                </div>
                
                <% } %>
                <p>
                 
            </p>
        </fieldset>

        <fieldset>
               Name:
               <%: Html.EditorFor(model => model.treeName)%>
               <%: Html.ValidationMessageFor(model => model.treeName)%> 
              
            <input type="submit" value="Save" />
            <br />
            <p>
            Trees:
            </p>
               <div id ="fileList">
                    <%for (int j = 0; j < Model.fileList.Count; ++j)
                    { %>
                        <div style="margin-bottom:5px;">
                         <%: Html.ActionLink(Model.fileList[j], "Index", new { id = Model.prop.ID, treeName = Model.fileList[j] })%>
                        </div>
                    <% } %>

                    <br />
                    <br />
                     <%: Html.ActionLink("Download", "generate", new { id = Model.prop.ID, filename = Model.treeName })%>
    </div>
        </fieldset>

      

    <% } %>
    </div>


    </div>
</body>
</html>


<script id="shader-fs" type="x-shader/x-fragment">
  precision mediump float;

  void main(void) {
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  }
</script>

<script id="shader-vs" type="x-shader/x-vertex">
  attribute vec3 aVertexPosition;

  uniform mat4 uMVMatrix;
  uniform mat4 uPMatrix;

  void main(void) {
    gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);
  }
</script>