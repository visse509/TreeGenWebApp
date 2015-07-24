<%@ Page Language="C#" Inherits="System.Web.Mvc.ViewPage<WebTreeGen.Models.Properties>" %>

<!DOCTYPE html>

<html>
<head runat="server">
    <title>Delete</title>
</head>
<body>
    <h3>Are you sure you want to delete this?</h3>
    <fieldset>
        <legend>Properties</legend>
    
        <div class="display-label">nSegments</div>
        <div class="display-field">
            <%: Html.DisplayFor(model => model.nSegments) %>
        </div>
    
        <div class="display-label">segmentLength</div>
        <div class="display-field">
            <%: Html.DisplayFor(model => model.segmentLength) %>
        </div>
    
        <div class="display-label">nEdges</div>
        <div class="display-field">
            <%: Html.DisplayFor(model => model.nEdges) %>
        </div>
    
        <div class="display-label">girth</div>
        <div class="display-field">
            <%: Html.DisplayFor(model => model.girth) %>
        </div>
    
        <div class="display-label">nLeafs</div>
        <div class="display-field">
            <%: Html.DisplayFor(model => model.nLeafs) %>
        </div>
    </fieldset>
    <% using (Html.BeginForm()) { %>
        <p>
            <input type="submit" value="Delete" /> |
            <%: Html.ActionLink("Back to List", "Index") %>
        </p>
    <% } %>
    
</body>
</html>
