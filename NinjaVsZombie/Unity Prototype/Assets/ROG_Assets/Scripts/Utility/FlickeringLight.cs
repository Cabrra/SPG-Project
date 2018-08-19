using UnityEngine;
using System.Collections;

public class FlickeringLight : MonoBehaviour
{
	void Update () 
	{
		if(Random.value < 0.2f) // 20% chance
			light.intensity = Random.Range(1.0f, 1.5f);
	}
}
