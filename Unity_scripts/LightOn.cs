using UnityEngine;
using System.Collections;

public class LightOn : MonoBehaviour {

	// Use this for initialization
	void Start ()
    {
	
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (TimeManager.LightsTurnOn)
        {
            light.enabled = true;
        }
        else
        {
            light.enabled = false;
        }
	}
}
