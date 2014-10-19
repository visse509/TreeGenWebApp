using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Data.Entity;
using System.Xml;
using System.IO;
using System.Diagnostics;

namespace WebTreeGen.Models
{
    public class Properties
    {
        public int ID { get; set; }
        public int nSegments { get; set; }
        public string segmentLength { get; set; }
        public int nEdges { get; set; }
        public string girth { get; set; }
        public int nLeafs { get; set; }
        public float rotation { get; set; }

        public static List<Properties> propertiesList = new List<Properties>();
        private static bool first = true;
        public static Process process;

        public Properties(string sessionId)
        {
            if (first)
            {
                first = false;
                getProperties(sessionId,"");
                
            }
        }
        public Properties()
        {
            if (first)
            {
                first = false;
                getProperties("noSession","");

            }
        }
        public void getProperties(string sessionId, string treeName)
        {
            propertiesList.Clear();
            bool stop = false;
            int id = 1;
            while (!stop)
            {
                Properties prop = new Properties(sessionId);
                if (!prop.readXml(id, sessionId, treeName))
                {
                    stop = true;
                }
                propertiesList.Add(prop);
                id++;
            }
        }
        public Properties getProperty(int id){
            return propertiesList.Find(x => x.ID == id);
        }

        public void save(string sessionId, string treeName)
        {
            XmlDocument xmlDoc = new XmlDocument();
            string dstPath = HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/" + sessionId + "/"+treeName+"_treeProp.xml")﻿﻿;
            string path = HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/treeProp.xml")﻿﻿;
            if (File.Exists(dstPath))
            {
                xmlDoc.Load(dstPath);
            }
            else
            {
                xmlDoc.Load(path);
            }
            XmlNodeList nodeList = xmlDoc.GetElementsByTagName("level");

            foreach (Properties prop in propertiesList)
            {

                foreach (XmlElement element in nodeList)
                {
                    if (element.Attributes["number"].Value == prop.ID.ToString())
                    {

                        XmlNode node = element.FirstChild;

                        node = node.NextSibling;
                        node.InnerText = prop.rotation.ToString();

                        node = node.NextSibling;
                        node.InnerText = prop.nSegments.ToString();

                        node = node.NextSibling;
                        node.InnerText = prop.segmentLength.ToString();

                        node = node.NextSibling;
                        node.InnerText = prop.nEdges.ToString();

                        node = node.NextSibling;
                        node.InnerText = prop.girth.ToString();

                        node = node.NextSibling;
                        node.InnerText = prop.nLeafs.ToString();
                    }

                }

            }

            xmlDoc.Save(dstPath);
            getProperties(sessionId,treeName);
        }

        public bool readXml(int id,string sessionId,string treeName)
        {
            XmlDocument xmlDoc = new XmlDocument();
            string path = HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/treeProp.xml")﻿﻿;
            if (Directory.Exists(HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/" + sessionId + "/")﻿﻿))
            {
                path = HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/" + sessionId + "/"+treeName+"_treeProp.xml")﻿﻿;
            }
           
            xmlDoc.Load(path);
            XmlNodeList nodeList = xmlDoc.GetElementsByTagName("level");
            foreach (XmlElement element in nodeList)
            {
                if (element.Attributes["number"].Value == id.ToString())
                {
                    this.ID = id;

                    XmlNode node = element.FirstChild;

                    node = node.NextSibling;
                    this.rotation = Convert.ToInt32(node.InnerText);

                    node = node.NextSibling;
                    this.nSegments = Convert.ToInt32(node.InnerText);

                    node = node.NextSibling;
                    this.segmentLength = node.InnerText;

                    node = node.NextSibling;
                    this.nEdges = Convert.ToInt32(node.InnerText);

                    node = node.NextSibling;
                    this.girth = node.InnerText;

                    node = node.NextSibling;
                    this.nLeafs = Convert.ToInt32(node.InnerText);

                    return true;

                }
            }
            return false;
        }

        public static int addLevel()
        {
            Properties prop = new Properties();
            return 1;
        }

        public static void createTree(string sessionId, string treeName)
        {
            string exePath = HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/treeGenDLL.exe")﻿﻿;
            string goPath = HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/go");
            string resultPath = HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/"+sessionId+"/" + treeName);
            string treePropPath = sessionId + "/"+treeName+"_treeProp.xml";
            string treeStringPath = sessionId + "/" + treeName + "_treeString.xml";
            string resPath = sessionId + "/" + treeName;


            System.IO.File.Delete(resultPath);
            System.IO.File.Delete(goPath);
            ProcessStartInfo startInfo = new ProcessStartInfo(exePath);
            string args = "\"" + treePropPath + "\"" + " " + "\"" + treeStringPath + "\"" + " " + "\"" + resPath + "\"";
            startInfo.Arguments = args;
            startInfo.WorkingDirectory = HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/");
            process = System.Diagnostics.Process.Start (startInfo);
            process.WaitForExit();

        }

    }
    public class PropertiesDbContext : DbContext
    {
        public DbSet<Properties> Properties { get; set; }
    }
}

