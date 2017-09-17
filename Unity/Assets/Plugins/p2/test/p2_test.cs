using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Text;
using System.IO; 
using System;

/* 
 * Tests p2 plugin using the same test data as the original repository
 * 
 * results (printed to Debug warning console) should be:
 * 		singles: 				-1.16995 	0.0040717 	1.31736
 * 		multi: 					-1.20176 	0.0260013 	1.28359
 * 		equal spacing: 			-1.14229 	0.02191 	1.31557
 * 		dense equal spacing:	-1.173 		0.0158448 	1.34372
 * 
*/

public class p2_test : MonoBehaviour {

	protected p2_plugin p2_multi;
	protected p2_plugin p2_10;
	protected p2_plugin p2_50;
	protected p2_plugin p2_90;
	protected p2_plugin p2_equal;
	protected p2_plugin p2_denseequal;

	// Use this for initialization
	void Start () {
		p2_10 = new p2_plugin();
		p2_10.AddQuantile (0.1);

		p2_50 = new p2_plugin();
		p2_50.AddQuantile (0.5);

		p2_90 = new p2_plugin();
		p2_90.AddQuantile (0.9);

		p2_multi = new p2_plugin();
		p2_multi.AddQuantile (0.1);
		p2_multi.AddQuantile (0.5);
		p2_multi.AddQuantile (0.9);

		p2_equal = new p2_plugin();
		p2_equal.AddEqualSpacing (10);

		p2_denseequal = new p2_plugin();
		p2_denseequal.AddEqualSpacing (20);

		LoadFile (Application.dataPath + "/Plugins/p2/test/data.txt");

		Debug.LogWarning("P^2 singles = " + p2_10.Result(0.1) + "  " + p2_50.Result(0.5) + "  " + p2_90.Result(0.9));
		Debug.LogWarning("P^2 multi: " + p2_multi.Result(0.1) + "  " +  p2_multi.Result(0.5) + "  " +  p2_multi.Result(0.9));
		Debug.LogWarning("P^2 equal = " + p2_equal.Result( 0.1 ) + "  " + p2_equal.Result( 0.5 )  + "  " + p2_equal.Result( 0.9 ) );
		Debug.LogWarning("P^2 denseequal = " + p2_denseequal.Result( 0.1 ) + "  " + p2_denseequal.Result( 0.5 )  + "  " + p2_denseequal.Result( 0.9 ) );

	}

	private bool LoadFile(string fileName){
		try{
			string line;
			// create new StreamReader
			StreamReader reader = new StreamReader(fileName, Encoding.Default);
			// using "using" statement to clean up instead of relying on garbage collection
			using (reader){
				// while lines left
				do{
					line = reader.ReadLine();
					if (line != null){
						double data = Double.Parse(line);
						p2_multi.AddData(data);
						p2_10.AddData(data);
						p2_50.AddData(data);
						p2_90.AddData(data);
						p2_equal.AddData(data);
						p2_denseequal.AddData(data);
					}
					else{
						Debug.Log("Done reading file: " + fileName);
					}
				}
				while (line != null);
				// close reader and return true    
				reader.Close();
				return true;
			}
		}
		catch (Exception e){
			Console.WriteLine("{0}\n", e.Message);
			return false;
		}
	}
}
