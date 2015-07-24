<%@ Page Language="C#" Inherits="System.Web.Mvc.ViewPage<WebTreeGen.Models.Properties>" %>

<!DOCTYPE html>

<html>
<head runat="server">
    <title>Details</title>
</head>
<body>
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
    <p>
    
        <%: Html.ActionLink("Edit", "Edit", new { id=Model.ID }) %> |
        <%: Html.ActionLink("Back to List", "Index") %>
    </p>
</body>
</html>
