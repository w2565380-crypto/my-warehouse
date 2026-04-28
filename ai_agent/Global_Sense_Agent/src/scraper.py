from playwright.async_api import async_playwright

async def fetch_amazon_data(url: str):
    async with async_playwright() as p:
        browser = await p.chromium.launch(headless=False)
        context = await browser.new_context()
        page = await context.new_page()

        await page.goto(url)
        await page.wait_for_timeout(2000)

        # ✅ 提取关键数据（示例：Books to Scrape）
        titles = await page.locator("h3 a").all_inner_texts()
        prices = await page.locator(".price_color").all_inner_texts()

        await browser.close()

        # ✅ 拼成干净文本（给 LLM）
        result = []
        for t, p in zip(titles[:10], prices[:10]):
            result.append(f"商品: {t} | 价格: {p}")

        return "\n".join(result)