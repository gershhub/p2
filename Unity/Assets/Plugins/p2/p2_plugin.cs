using UnityEngine;
using System.Runtime.InteropServices;
using System.Collections;
using System.Collections.Generic;
using System;

/* 	
 * P^2 algorithm implemented as a Unity native code plugin library.
 * 
 * Ported to C (and wrapped for Unity) by Gershon Dublon.
 * Original C++ library by user "absmall" retrieved from https://github.com/absmall/p2
 * 
 * A test script (p2_test.cs) uses the same input data as the original library and 
 * seems to arrive at the same results.
 * 
 * From the github:
 * 	"This program implements the P^2 algorithm as documented in 'The P-Square Algorithm 
 * 	for Dynamic Calculation of Percentiles and Histograms without Storing Observations,'
 * 	Communications of the ACM, October 1985 by R. Jain and I. Chlamtac. Both the point 
 * 	method and histogram method are implemented."
*/

public class  p2_plugin {
#if UNITY_IPHONE
    private const string PLUGIN_NAME = "__Internal";
#else
    private const string PLUGIN_NAME = "p2";
#endif

    [DllImport (PLUGIN_NAME)]
    private static extern System.IntPtr p2_create();

    [DllImport (PLUGIN_NAME)]
    private static extern void p2_destroy(System.IntPtr p2);

    [DllImport (PLUGIN_NAME)]
    private static extern void  p2_add_quantile(System.IntPtr p2, double quantile);

    [DllImport (PLUGIN_NAME)]
    private static extern void p2_add_equal_spacing(System.IntPtr p2, int n);

    [DllImport (PLUGIN_NAME)]
    private static extern void p2_add(System.IntPtr p2, double data);

    [DllImport (PLUGIN_NAME)]
    private static extern double p2_result(System.IntPtr p2, double quantile);

    System.IntPtr p2;
    
    public p2_plugin() {
        p2 = p2_create();
		Debug.Log("Creating P^2 (p2) plugin instance " + p2);
    }

	public void AddQuantile(double quantile){
		p2_add_quantile (p2, quantile);
	}

	public void AddEqualSpacing(int n){
		p2_add_equal_spacing(p2, n);
	}

	public void AddData(double data){
		p2_add(p2, data);
	}

	public double Result(double quantile){
		return p2_result(p2, quantile);
	}

    ~p2_plugin() {
		Debug.Log("Destroying P^2 (p2) plugin instance");
        p2_destroy(p2);
    }

}
