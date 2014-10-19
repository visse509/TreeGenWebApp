using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Data.Entity;
using System.Xml;
using System.IO;

namespace WebTreeGen.Models
{
    public class LSystem
    {
     //   public List<string> alphabet { get; set; }
        public List<string> ID { get; set; }
        public List<string> axiom { get; set; }
        public Dictionary<char,string> pRules { get; set; }
        private static bool lfirst = true;
        public LSystem()
        {
            axiom = new List<string>();
            pRules = new Dictionary<char, string>();

        }

        public void readXML(string sessionId, string treeName)
        {
            axiom.Clear();
            pRules.Clear();
            XmlDocument xmlDoc = new XmlDocument();
            string dstPath = HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/" + sessionId + "/" + treeName + "_treeString.xml")﻿﻿;
            string path = HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/treeString.xml")﻿﻿;
            if (File.Exists(dstPath))
            {
                xmlDoc.Load(dstPath);
            }
            else
            {
                xmlDoc.Load(path);
            }
            XmlNodeList nodeList = xmlDoc.GetElementsByTagName("axiom");
            foreach (XmlElement element in nodeList)
            {
                axiom.Add(element.Attributes[0].Value);
            }

            nodeList = xmlDoc.GetElementsByTagName("productionRule");
            foreach (XmlElement element in nodeList)
            {
                XmlNode node = element.FirstChild;
                pRules[Convert.ToChar(element.Attributes[0].Value)] = node.InnerText;
            }

        }

        public void save(string sessionId, string treeName)
        {
            
            XmlDocument xmlDoc = new XmlDocument();
            string path = HttpContext.Current.ApplicationInstance.Server.MapPath("~/App_Data/treeString.xml")﻿﻿;
            string dstPath = HttpContext.Current.ApplicationInstance.Server.MapPath("~/users/" + sessionId + "/" + treeName + "_treeString.xml")﻿﻿;

            System.IO.File.WriteAllText(dstPath, string.Empty);
            XmlElement DocumentElement = xmlDoc.CreateElement("root");
            XmlNode axiomNode =  xmlDoc.CreateNode(XmlNodeType.Element,"axiomList",null);
            foreach (string item in axiom)
            {
                if (item != "")
                {
                    XmlNode nodeTitle = xmlDoc.CreateElement("axiom");
                    XmlAttribute attribute = xmlDoc.CreateAttribute("value");
                    attribute.InnerText = item;
                    nodeTitle.Attributes.Append(attribute);
                    axiomNode.AppendChild(nodeTitle);
                }
            }
            DocumentElement.AppendChild(axiomNode);

            XmlNode rulesNode = xmlDoc.CreateNode(XmlNodeType.Element, "productionRuleList", null);
            foreach(KeyValuePair<char, string> item in pRules)
            {
                XmlNode nodeTitle = xmlDoc.CreateElement("productionRule");
                XmlAttribute attribute = xmlDoc.CreateAttribute("value");
                attribute.InnerText = Convert.ToString(item.Key);
                nodeTitle.Attributes.Append(attribute);
                nodeTitle.InnerText = item.Value;
                rulesNode.AppendChild(nodeTitle);
            }

            DocumentElement.AppendChild(rulesNode);
            xmlDoc.AppendChild(DocumentElement);
            xmlDoc.Save(dstPath);
            xmlDoc.Save(path);
        }
    }
    public class LSystemDbContext : DbContext
    {
        public DbSet<Properties> LSystem { get; set; }
    }
    
}