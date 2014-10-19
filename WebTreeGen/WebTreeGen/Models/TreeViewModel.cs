using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.IO;

namespace WebTreeGen.Models
{
    public class TreeViewModel
    {
        public Properties prop {get; set;}
        public LSystem sys { get; set; }
        public List<String> fileList { get; set; }
        public string treeName { get; set; }

        public TreeViewModel(string sessionId)
        {
            prop = new Properties(sessionId);
            sys = new LSystem();
            fileList = new List<String>();
        }
        public TreeViewModel()
        {
            prop = new Properties();
            sys = new LSystem();
        }

        public TreeViewModel getView(int id, int p,string sessionId,string treeName)
        {
            TreeViewModel res = new TreeViewModel(sessionId);
            res.prop.getProperties(sessionId,treeName);
            res.prop = prop.getProperty(id);
            sys.readXML(sessionId, treeName);
            res.sys = sys;
            res.treeName = treeName;
            res.fileList = getUserFiles(sessionId);

            return res;
        }

        private List<string> getUserFiles(string sessionId)
        {
            List<String> res = new List<string>();
            var path = System.Web.HttpContext.Current.Server.MapPath("~/Users/"+sessionId+"/");

            if (Directory.Exists(path))
            {
                string[] fileEntries = Directory.GetFiles(path);
                foreach (string file in fileEntries)
                {
                    if (file.Substring(file.Length - 4) == ".fbx")
                    {
                        res.Add(Path.GetFileNameWithoutExtension(file));
                    }
                }
            }

            return res;
        }

        public void save(string sessionId,string treeName)
        {
            var path = System.Web.HttpContext.Current.Server.MapPath("~/Users/"+sessionId+"/");
            Directory.CreateDirectory(path);   
            prop.save(sessionId,treeName);
            sys.save(sessionId, treeName);
            
        }
    }
}