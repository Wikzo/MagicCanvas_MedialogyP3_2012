using UnityEngine;
using System.Collections;
using Random = UnityEngine.Random;

public class Gift : MonoBehaviour
{

    public Material[] GiftMaterials;

	// Use this for initialization
	void Start ()
	{
	    int randomMaterial = Random.Range(0, GiftMaterials.Length);
	    transform.renderer.material = GiftMaterials[randomMaterial];

        gameObject.rigidbody.AddForce(Vector3.down * Random.Range(50f, 100f), ForceMode.VelocityChange);
        gameObject.rigidbody.AddForce(Vector3.left * Random.Range(-10f, 10f), ForceMode.VelocityChange);
	    StartCoroutine(DieAfterSomeTime());
	}

    void Update()
    {
    }
	
	IEnumerator DieAfterSomeTime()
	{
	    float dieTime = Random.Range(50f, 300f);
        yield return new WaitForSeconds(dieTime);
        Destroy(gameObject.renderer.material);
        Destroy(gameObject);
	}
}
