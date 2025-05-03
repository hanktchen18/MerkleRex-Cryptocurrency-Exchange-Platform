This document demonstrates pseudocode of the matching algorithm between ask and bid orders:



bids = orderbook.bids
asks = orderbook.asks

sales = []
<!-- Encourage bids high, so the highest bid may get the lowest ask -->
<!-- higest bid may get to save a lot (bid price - ask price matched), lowest ask may to sell ASAP -->
sort asks lowest first
sort bids highest first

for ask in asks:
    for bid in bids:
        <!-- We have a match -->
        if bid.price >= ask.price:
            sale = new order()
            sale.price = ask.price
            <!-- 1st scenario: ask = bid -->
            if bid.amount == ask.amount:
                sale.amount = (ask/bid) amount
                sales.append(sale)
                <!-- To make sure the bid is not processed again -->
                bid.amount = 0
                <!-- break from processing this ask -->
                <!-- Go to next ask -->
                break
            <!-- 2nd scenario: bid > ask (not enough to buy) -->
            if bid.amount > ask.amount:
                sale.amount = bid.amount
                sales.append(sale)
                bid.amount  = bid.amount - ask.amount
                <!-- ask is completely gone, need to process next ask to fulfill this bid -->
                break
            <!-- 3rd scenario: ask > bid (not sell the whole thing) -->
            if ask.amount > bid.amount:
                sale.amount = ask.amount
                sales.append(sale)
                ask.amount = ask.amount - bid.amount
                bid.amount = 0
                <!-- bid is completely gone, process next bids to fulfill this ask -->
                continue