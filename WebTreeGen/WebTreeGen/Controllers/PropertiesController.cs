using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using WebTreeGen.Models;
using System.Web.SessionState;
using System.IO;

namespace WebTreeGen.Controllers
{ 
    public class PropertiesController : Controller
    {
        private PropertiesDbContext db = new PropertiesDbContext();
        public TreeViewModel tree = new TreeViewModel();

        //
        // GET: /Properties/

        public ViewResult Index(string id, string treeName)
        {
            string sessionId = Session.SessionID;
            Session["ID"] = "ID";
            if (id == null)
            {
                id = "1";
            }
            TreeViewModel res = tree.getView(Convert.ToInt32(id), 0, sessionId,treeName);
            res.save(sessionId,treeName);
            return View(res);
        }

        [HttpPost]
        public ActionResult Index(TreeViewModel tree,Dictionary<char,string> pRules)
        {
            string sessionId = Session.SessionID;
            if (tree.treeName == null)
            {
                tree.treeName = "default";
            }
            Properties.propertiesList[tree.prop.ID] = tree.prop;
            tree.sys.pRules = pRules;
            tree.save(sessionId,tree.treeName);
            Properties.createTree(sessionId,tree.treeName);
            return View(tree.getView(Convert.ToInt32(tree.prop.ID), 0,sessionId,tree.treeName));
        }

        public ActionResult generate(string filename)
        {
            if (filename == null)
            {
                filename = "res";
            }
            string sessionId = Session.SessionID;
            string resPath = System.Web.HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/" + Session.SessionID + "/"+ filename +".fbx")﻿﻿;
            System.IO.FileStream fs = System.IO.File.OpenRead(resPath);
            byte[] data = new byte[fs.Length];
            int br = fs.Read(data, 0, data.Length);

            if (br != fs.Length)
                throw new System.IO.IOException(resPath);

            fs.Close();
            return File(

                data, System.Net.Mime.MediaTypeNames.Application.Octet, filename+".fbx");
        }
        //
        // GET: /Properties/Create

        public ActionResult getData(string input)
        {
            if (Properties.process != null)
            {
                Properties.process.WaitForExit();
            }
            string path = System.Web.HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/" + Session.SessionID + "/" + input + ".xml")﻿﻿;
            if(!System.IO.File.Exists(path)){
                path = System.Web.HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/modelData.xml")﻿﻿;
            }
            string xmlString = System.IO.File.ReadAllText(path);
            return this.Content(xmlString, "text");
        }

        protected override void Dispose(bool disposing)
        {
            db.Dispose();
            base.Dispose(disposing);
        }
    }
}