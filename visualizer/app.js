let allocator = null;
let selectedBlock = null;

async function loadAllocator() {
	const res = await fetch("allocator.json");
	if (!res.ok)
		throw new Error("Could not load allocator.json");

	allocator = await res.json();

	renderZones("tiny");
	renderZones("small");
	renderZones("large");
}

function hexToNumber(address) {
	return parseInt(address, 16);
}

function isAligned(address, alignment) {
	const value = hexToNumber(address);
	return value % alignment === 0;
}

function renderZone(zone) {
	const container = document.createElement("div");
	container.className = "zone";

	const header = document.createElement("div");
	header.className = "zone-header";
	header.innerHTML = `
		<h3>Zone ${zone.address}</h3>

		<div class="zone-info">
			Size: ${zone.zoneSize} bytes |
			Block size: ${zone.blockSize} bytes
		</div>
	`;

	const blocks = document.createElement("div");
	blocks.className = "blocks";

	zone.blocks.forEach(block => {
		const element = document.createElement("div");
		element.className =
			"block " + (block.used ? "used" : "free");

		const aligned =
			isAligned(block.address, allocator.alignment);

		element.innerHTML = `
			<div class="block-number">
				#${block.index}
			</div>

			<div>
				${block.used ? "USED" : "FREE"}
			</div>

			<div class="block-address">
				${block.address}
			</div>

			<div class="${aligned ? "aligned" : "misaligned"}">
				${aligned ? "aligned" : "misaligned"}
			</div>
		`;

		element.addEventListener("click", () => {
			showBlockDetails(zone, block, container);
		});

		blocks.appendChild(element);
	});

	container.appendChild(header);
	container.appendChild(blocks);

	return container;
}

function renderZones(type) {
	const container =
		document.getElementById(`${type}-zones`);
	container.innerHTML = "";

	(allocator[type] || []).forEach(zone => {

		container.appendChild(
			renderZone(zone)
		);
	});
}

function showBlockDetails(zone, block, zoneContainer) {
	const existingDetails = document.querySelector(".allocation-details")
	if (existingDetails)
		existingDetails.remove();

	if (selectedBlock === block) {
		if (existingDetails)
			existingDetails.remove();
		selectedBlock = null;
		return;
	}

	if (existingDetails)
		existingDetails.remove();

	selectedBlock = block;

	const aligned = isAligned(block.address, allocator.alignment);

	const details = document.createElement("div");
	details.className = "allocation-details";
	details.innerHTML = `
		<div class="detail-header">
			<h3>Allocation details</h3>
			<button
				type="button"
				class="close-details"
				aria-label="Close"
			>X</button>
		</div>

		<div class="detail-grid">

			<div>
				<div class="detail-label">
					Zone
				</div>

				<div class="detail-value">
					${zone.address}
				</div>
			</div>

			<div>
				<div class="detail-label">
					Block
				</div>

				<div class="detail-value">
					#${block.index}
				</div>
			</div>

			<div>
				<div class="detail-label">
					Address
				</div>

				<div class="detail-value">
					${block.address}
				</div>
			</div>

			<div>
				<div class="detail-label">
					Block size
				</div>

				<div class="detail-value">
					${zone.blockSize} bytes
				</div>
			</div>

			<div>
				<div class="detail-label">
					Requested
				</div>

				<div class="detail-value">
					${block.used
						? block.requested + " bytes"
						: "-"}
				</div>
			</div>

			<div>
				<div class="detail-label">
					Alignment
				</div>

				<div class="detail-value">
					<span class="status ${aligned ? "ok" : "error"}">
						${aligned
							? "ALIGNED"
							: "MISALIGNED"}
					</span>
				</div>
			</div>

		</div>
	`;

	zoneContainer.insertAdjacentElement(
		"afterend",
		details
	);

	details
		.querySelector(".close-details")
		.addEventListener("click", () => {
			details.remove();
		});
}

loadAllocator().catch(error => {
	console.error(error);
})
