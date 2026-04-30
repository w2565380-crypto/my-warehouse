from playwright.async_api import async_playwright


async def fetch_amazon_data(url: str):
    async with async_playwright() as p:
        browser = await p.chromium.launch(headless=True)

        context = await browser.new_context(
            user_agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64)",
            viewport={"width": 1280, "height": 800}
        )

        page = await context.new_page()

        print(f"🔍 正在抓取: {url}")

        try:
            await page.goto(url, timeout=30000)
            await page.wait_for_load_state("domcontentloaded")

            # 👉 等一会，防止没加载完
            await page.wait_for_timeout(1000)

            # ----------------------------
            # 🧠 自动判断页面类型
            # ----------------------------
            products = await page.locator(".product_pod").all()

            # ============================
            # ✅ 1. 列表页
            # ============================
            if len(products) > 0:
                print("📦 检测到列表页")

                results = []

                for p_item in products[:10]:
                    title_el = p_item.locator("h3 a")
                    price_el = p_item.locator(".price_color")

                    title = await title_el.get_attribute("title")
                    price = await price_el.inner_text()

                    results.append(f"商品: {title} | 价格: {price}")

                await browser.close()
                return "\n".join(results)

            # ============================
            # ✅ 2. 详情页
            # ============================
            else:
                print("📄 检测到详情页")

                title = await page.locator("h1").inner_text()
                price = await page.locator(".price_color").inner_text()

                # 描述可能不存在，要容错
                description = ""
                desc_locator = page.locator("#product_description ~ p")
                if await desc_locator.count() > 0:
                    description = await desc_locator.inner_text()

                result = f"""
商品: {title}
价格: {price}
描述: {description}
"""

                await browser.close()
                return result.strip()

        except Exception as e:
            print(f"❌ 抓取失败: {e}")
            await browser.close()
            return ""