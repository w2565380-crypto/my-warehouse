import feedparser

GOOGLE_NEWS_RSS = (
    "https://news.google.com/rss/search?q=stock+market"
)

def get_latest_news():

    feed = feedparser.parse(
        GOOGLE_NEWS_RSS
    )

    headlines = []

    for entry in feed.entries[:5]:

        headlines.append({
            "title": entry.title,
            "link": entry.link
        })

    return headlines