using UnityEngine;
using System.Collections;
using Random = UnityEngine.Random;

public class Snowball : MonoBehaviour
{
    private Renderer snowBallRenderer;
    Vector3 defaultPos = new Vector3(109f, 203f, -9f);
    Vector3 defaultScale = new Vector3(9f, 9f, 9f);
    public float growSize = 1f;
    private bool isRespawning;
    private float maxSize = 80;

    public GameObject fireworkPrefab;

	// Use this for initialization
	void Start ()
	{
	    snowBallRenderer = gameObject.renderer;
        transform.position = defaultPos;
	    transform.localScale = defaultScale;
	    isRespawning = false;

	    StartCoroutine(RespawnSnowBall(true));
	}
	
    IEnumerator RespawnSnowBall(bool firstTime)
    {
        if (!firstTime)
            Instantiate(fireworkPrefab, transform.position, Quaternion.identity);

        isRespawning = true;
        transform.localScale = defaultScale;
        transform.position = defaultPos;
        GetComponent<Rigidbody>().isKinematic = true;
        GetComponent<Rigidbody>().useGravity = false;

        float randomSpawnTime;
        randomSpawnTime = !firstTime ? Random.Range(5f, 200f) : 5f;
        //print("snowball spawn time: " + randomSpawnTime);
        yield return new WaitForSeconds(randomSpawnTime);

        GetComponent<Rigidbody>().isKinematic = false;
        GetComponent<Rigidbody>().useGravity = true;
    }

    // Update is called once per frame
	void Update ()
	{

        rigidbody.AddForce(Vector3.down * 9.8f, ForceMode.Acceleration);


        if (!isRespawning && transform.localScale.x >= maxSize)
        {
            StartCoroutine(RespawnSnowBall(false));
        }

	    if (!isRespawning && !snowBallRenderer.isVisible)
	    {
	        StartCoroutine(RespawnSnowBall(false));
	    }
	}

    void OnCollisionEnter(Collision collision)
    {
        isRespawning = false;

        if (collision.transform.tag != "Wall" && collision.transform.tag != "Gift")
            transform.localScale = new Vector3(transform.localScale.x + growSize, transform.localScale.y + growSize, transform.localScale.z + growSize);
    }
}
