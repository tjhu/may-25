using System;
using System.Runtime.InteropServices;
using libsbmlcs;

namespace CCR
{
    [ComVisible(true)]
    //[Guid("e515aeaf-881a-4811-8cb8-956ce5f6bfe3")]
    public interface IMyClass
    {
        string GetData(string input);
    }
}

namespace CCR
{
    [ComVisible(true)]
    [ClassInterface(ClassInterfaceType.None)]
    //[Guid("8ebb2fc5-7ce0-41bb-98e0-aea29481d809")]
    public class MyClass : IMyClass
    {

        public string GetData(string input)
        {
            String input_mathml = input;
            //String sample = "<?xml version='1.0' encoding='UTF-8'?>"
            //+ "<math xmlns='http://www.w3.org/1998/Math/MathML'>"
            //+ "  <apply> <plus/> <cn> 1 </cn>"
            //+ "                  <apply> <ci> f </ci> <ci> x </ci> </apply>"
            //+ "  </apply>"
            //+ "</math>";
            System.Windows.Forms.MessageBox.Show("Input:\n" + input_mathml + "\n");
            ASTNode ast_result = libsbmlcs.libsbml.readMathMLFromString(input_mathml);
            String ast_as_string = libsbmlcs.libsbml.formulaToString(ast_result);
            System.Windows.Forms.MessageBox.Show("Output:\n" + ast_as_string);
            return ast_as_string;
        }
    }
}