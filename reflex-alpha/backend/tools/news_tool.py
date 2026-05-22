import feedparser

RSS_URL = (
    "https://news.google.com/rss/search?q=stock+market"
)

def get_latest_news():

    feed = feedparser.parse(
        RSS_URL
    )

    news = []

    for entry in feed.entries[:5]:

        news.append({

            "title": entry.title,

            "link": entry.link
        })

    # fallback
    if len(news) == 0:

        news = [
            {
                "title": (
                    "NVIDIA faces new export restrictions to China"
                ),
                "link": ""
            }
        ]

    return news