import re

def on_page_markdown(markdown, config, **kwargs):
    base = config.repo_url.rstrip("/")
    branch = "master"

    return re.sub(
        r"\]\(repo:([^)]+)\)",
        lambda m: f"]({base}/blob/{branch}/{m.group(1)})",
        markdown,
    )
