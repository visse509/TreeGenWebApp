<%@ Page Language="C#" Inherits="System.Web.Mvc.ViewPage<WebTreeGen.Models.Properties>" %>

<!DOCTYPE html>

<html>
<head runat="server">
    <title>Create</title>
</head>
<body>
    <script src="<%: Url.Content("~/Scripts/jquery-1.5.1.min.js") %>" type="text/javascript"></script>
    <script src="<%: Url.Content("~/Scripts/jquery.validate.min.js") %>" type="text/javascript"></script>
    <script src="<%: Url.Content("~/Scripts/jquery.validate.unobtrusive.min.js") %>" type="text/javascript"></script>
    
    <% using (Html.BeginForm()) { %>
        <%: Html.ValidationSummary(true) %>
        <fieldset>
            <legend>Properties</legend>
    
            <div class="editor-label">
                <%: Html.LabelFor(model => model.nSegments) %>
            </div>
            <div class="editor-field">
                <%: Html.EditorFor(model => model.nSegments) %>
                <%: Html.ValidationMessageFor(model => model.nSegments) %>
            </div>
    
            <div class="editor-label">
                <%: Html.LabelFor(model => model.segmentLength) %>
            </div>
            <div class="editor-field">
                <%: Html.EditorFor(model => model.segmentLength) %>
                <%: Html.ValidationMessageFor(model => model.segmentLength) %>
            </div>
    
            <div class="editor-label">
                <%: Html.LabelFor(model => model.nEdges) %>
            </div>
            <div class="editor-field">
                <%: Html.EditorFor(model => model.nEdges) %>
                <%: Html.ValidationMessageFor(model => model.nEdges) %>
            </div>
    
            <div class="editor-label">
                <%: Html.LabelFor(model => model.girth) %>
            </div>
            <div class="editor-field">
                <%: Html.EditorFor(model => model.girth) %>
                <%: Html.ValidationMessageFor(model => model.girth) %>
            </div>
    
            <div class="editor-label">
                <%: Html.LabelFor(model => model.nLeafs) %>
            </div>
            <div class="editor-field">
                <%: Html.EditorFor(model => model.nLeafs) %>
                <%: Html.ValidationMessageFor(model => model.nLeafs) %>
            </div>
    
            <p>
                <input type="submit" value="Create" />
            </p>
        </fieldset>
    <% } %>
    
    <div>
        <%: Html.ActionLink("Back to List", "Index") %>
    </div>
</body>
</html>
